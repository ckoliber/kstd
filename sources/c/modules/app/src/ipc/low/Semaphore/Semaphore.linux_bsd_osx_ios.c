#include <ipc/low/Semaphore.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <dsa/low/String.h>
#include <errno.h>
#include <fcntl.h>
#include <ipc/low/Mutex.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <pthread.h>
#include <sys/mman.h>

struct Semaphore_ {
    Semaphore self;
    void* memory;
    String* name;
};

// link methods
int semaphore_init(struct Semaphore* self, int value);
int semaphore_wait(struct Semaphore* self, uint_64 timeout);
int semaphore_post(struct Semaphore* self);
int semaphore_get(struct Semaphore* self);

// local methods
void* semaphore_anonymous_new();
void semaphore_anonymous_free(void* memory);
void* semaphore_named_new(char* name);
void semaphore_named_free(void* memory, char* name);

void* semaphore_anonymous_new() {
    // alocate mutex and cond and svalue
    void* result = heap_alloc(sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int));

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = result;
    pthread_mutex_t* cond = result + sizeof(pthread_mutex_t);
    int* svalue = result + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

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

    // init svalue
    *svalue = 0;

    return result;
}
void semaphore_anonymous_free(void* memory) {
    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = memory;
    pthread_cond_t* cond = memory + sizeof(pthread_mutex_t);
    int* svalue = memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // destroy internal mutex and cond
    pthread_mutex_destroy(mutex);
    pthread_cond_destroy(cond);

    heap_free(memory);
}
void* semaphore_named_new(char* name) {
    // check share memory exists
    bool exists = true;
    int exists_fd = shm_open(name, O_CREAT | O_EXCL, 0660);
    if (exists_fd > 0) {
        // not exists, create it
        close(exists_fd);
        exists = false;
    }

    // alocate share mutex and cond and svalue and connections
    int fd = shm_open(name, O_CREAT | O_RDWR, 0660);
    void* result = mmap(NULL, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int) + sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    close(fd);

    // check error
    if (result == NULL || result == MAP_FAILED) {
        return NULL;
    }

    // get mutex and cond and svalue and connections address
    pthread_mutex_t* mutex = result;
    pthread_mutex_t* cond = result + sizeof(pthread_mutex_t);
    int* svalue = result + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);
    int* connections = result + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int);

    // create and init semaphore or open and increase connections
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

        // init share svalue
        *svalue = 0;
    } else {
        *connections += 1;
    }

    return result;
}
void semaphore_named_free(void* memory, char* name) {
    // get mutex and cond and svalue and connections address
    pthread_mutex_t* mutex = memory;
    pthread_mutex_t* cond = memory + sizeof(pthread_mutex_t);
    int* svalue = memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);
    int* connections = memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int);

    // destroy mutex and cond and share memory on close all connections
    if (*connections <= 1) {
        pthread_mutex_destroy(mutex);
        pthread_cond_destroy(cond);
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int) + sizeof(int));
        shm_unlink(name);
    } else {
        *connections -= 1;
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int) + sizeof(int));
    }
}

// implement methods
int semaphore_init(struct Semaphore* self, int value) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = semaphore_->memory;
    pthread_cond_t* cond = semaphore_->memory + sizeof(pthread_mutex_t);
    int* svalue = semaphore_->memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // init semaphore value
    int result = 0;
    *svalue = value;

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}
int semaphore_wait(struct Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = semaphore_->memory;
    pthread_cond_t* cond = semaphore_->memory + sizeof(pthread_mutex_t);
    int* svalue = semaphore_->memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // wait the pthread cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        while (*svalue == 0) {
            pthread_cond_wait(cond, mutex);
        }
        *svalue--;
        result = 0;
    } else {
        // timed
        bool timedout = false;
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;
        while (*svalue == 0) {
            if (pthread_cond_timedwait(cond, mutex, &time_out) == ETIMEDOUT) {
                timedout = true;
                break;
            }
        }
        if (!timedout) {
            *svalue--;
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}
int semaphore_post(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = semaphore_->memory;
    pthread_cond_t* cond = semaphore_->memory + sizeof(pthread_mutex_t);
    int* svalue = semaphore_->memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // signal the pthread cond
    int result = -1;
    *svalue++;
    if (pthread_cond_signal(cond) == 0) {
        result = 0;
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}
int semaphore_get(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = semaphore_->memory;
    pthread_cond_t* cond = semaphore_->memory + sizeof(pthread_mutex_t);
    int* svalue = semaphore_->memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // get semaphore value
    int result = *svalue;

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}

Semaphore* semaphore_new(char* name) {
    struct Semaphore_* semaphore_ = heap_alloc(sizeof(struct Semaphore_));

    // init private methods
    semaphore_->self.init = semaphore_init;
    semaphore_->self.wait = semaphore_wait;
    semaphore_->self.post = semaphore_post;
    semaphore_->self.get = semaphore_get;

    if (name == NULL) {
        semaphore_->name = NULL;

        // create internal mutex and cond and svalue
        semaphore_->memory = semaphore_anonymous_new();
    } else {
        semaphore_->name = string_new_concat(name, "/semaphore");

        // try acquire critical mutex
        if (critical != NULL) {
            critical->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share mutex and cond and svalue
        semaphore_->memory = semaphore_named_new(semaphore_->name->value(semaphore_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->release(critical);
        }
    }

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    if (semaphore_->name == NULL) {
        // destroy internal mutex and cond and svalue
        semaphore_anonymous_free(semaphore_->memory);
    } else {
        // try acquire critical mutex
        if (critical != NULL) {
            critical->acquire(critical, UINT_64_MAX);
        }

        // destroy and close or close internal share mutex and cond and svalue
        semaphore_named_free(semaphore_->memory, semaphore_->name->value(semaphore_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->release(critical);
        }

        string_free(semaphore_->name);
    }

    heap_free(semaphore_);
}

#endif