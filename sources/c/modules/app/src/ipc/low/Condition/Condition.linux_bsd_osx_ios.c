#include <ipc/low/Condition.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <dsa/low/String.h>
#include <fcntl.h>
#include <ipc/low/Mutex.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <pthread.h>
#include <sys/mman.h>

struct Condition_ {
    struct Condition self;
    void* memory;
    String* name;
};

// link methods
int condition_wait(struct Condition* self, uint_64 timeout);
int condition_signal(struct Condition* self, int count);

// local methods
void* condition_anonymous_new();
void condition_anonymous_free(void* memory);
void* condition_named_new(char* name);
void condition_named_free(void* memory, char* name);

void* condition_anonymous_new() {
    // alocate mutex
    void* result = heap_alloc(sizeof(pthread_mutex_t) + sizeof(pthread_cond_t));

    // get mutex and cond address
    pthread_mutex_t* mutex = result;
    pthread_mutex_t* cond = result + sizeof(pthread_mutex_t);

    // init mutex
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 0);
    pthread_mutex_init(mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);

    // init cond
    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, 0);
    pthread_cond_init(cond, &cattr);
    pthread_condattr_destroy(&cattr);

    return result;
}
void condition_anonymous_free(void* memory) {
    // get mutex and cond address
    pthread_mutex_t* mutex = memory;
    pthread_cond_t* cond = memory + sizeof(pthread_mutex_t);

    // destroy internal mutex
    pthread_mutex_destroy(mutex);
    pthread_cond_destroy(cond);

    heap_free(memory);
}
void* condition_named_new(char* name) {
    // check share memory exists
    bool exists = true;
    int exists_fd = shm_open(name, O_CREAT | O_EXCL, 0660);
    if (exists_fd > 0) {
        // not exists, create it
        close(exists_fd);
        exists = false;
    }

    // alocate share mutex and cond and connection
    int fd = shm_open(name, O_CREAT | O_RDWR, 0660);
    void* result = mmap(NULL, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    close(fd);

    // check error
    if (result == NULL || result == MAP_FAILED) {
        return NULL;
    }

    // get mutex and cond and connections address
    pthread_mutex_t* mutex = result;
    pthread_mutex_t* cond = result + sizeof(pthread_mutex_t);
    int* connections = result + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // create and init or open and increase connections
    if (!exists) {
        *connections = 1;

        // init share mutex
        pthread_mutexattr_t mattr;
        pthread_mutexattr_init(&mattr);
        pthread_mutexattr_setpshared(&mattr, 1);
        pthread_mutex_init(mutex, &mattr);
        pthread_mutexattr_destroy(&mattr);

        // init share cond
        pthread_condattr_t cattr;
        pthread_condattr_init(&cattr);
        pthread_condattr_setpshared(&cattr, 1);
        pthread_cond_init(cond, &cattr);
        pthread_condattr_destroy(&cattr);
    } else {
        *connections += 1;
    }

    return result;
}
void condition_named_free(void* memory, char* name) {
    // get mutex and cond and connections address
    pthread_mutex_t* mutex = memory;
    pthread_cond_t* cond = memory + sizeof(pthread_mutex_t);
    int* connections = memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // destroy mutex and cond and share memory on close all connections
    if (connections <= 1) {
        pthread_mutex_destroy(mutex);
        pthread_cond_destroy(cond);
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int));
        shm_unlink(name);
    } else {
        *connections -= 1;
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int));
    }
}

// implement methods
int condition_wait(struct Condition* self, uint_64 timeout) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // get mutex and cond address
    pthread_mutex_t* mutex = condition_->memory;
    pthread_cond_t* cond = condition_->memory + sizeof(pthread_mutex_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // wait the pthread cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_cond_wait(cond, mutex) == 0) {
            result = 0;
        }
    } else {
        // timed
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;
        if (pthread_cond_timedwait(cond, mutex, &time_out) == 0) {
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}
int condition_signal(struct Condition* self, int count) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // get mutex and cond address
    pthread_mutex_t* mutex = condition_->memory;
    pthread_cond_t* cond = condition_->memory + sizeof(pthread_mutex_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // signal the pthread cond
    int result = -1;
    if (count == INT_MAX) {
        // broadcast
        if (pthread_cond_broadcast(cond) == 0) {
            result = 0;
        }
    } else {
        // signal
        for (int cursor = 0; cursor < count; cursor++) {
            pthread_cond_signal(cond);
        }
        result = 0;
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}

Condition* condition_new(char* name) {
    struct Condition_* condition_ = heap_alloc(sizeof(struct Condition_));

    // init private methods
    condition_->self.wait = condition_wait;
    condition_->self.signal = condition_signal;

    if (name == NULL) {
        condition_->name = NULL;

        // create internal mutex
        condition_->memory = mutex_anonymous_new();
    } else {
        condition_->name = string_new_concat(name, "/condition");

        // try acquire critical mutex
        if (critical != NULL) {
            critical->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share mutex
        condition_->memory = mutex_named_new(condition_->name->value(condition_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->release(critical);
        }
    }

    return (Condition*)condition_;
}
void condition_free(Condition* condition) {
    struct Condition_* condition_ = (struct Condition_*)condition;

    if (condition_->name == NULL) {
        // destroy internal mutex and cond
        condition_anonymous_free(condition_->memory);
    } else {
        // try acquire critical mutex
        if (critical != NULL) {
            critical->acquire(critical, UINT_64_MAX);
        }

        // destroy and close or close internal share mutex and cond
        condition_named_free(condition_->memory, condition_->name->value(condition_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->release(critical);
        }

        string_free(condition_->name);
    }

    heap_free(condition_);
}

#endif