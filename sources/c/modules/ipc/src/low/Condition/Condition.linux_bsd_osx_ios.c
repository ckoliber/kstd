#include <low/Condition.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <fcntl.h>
#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/mman.h>

struct Condition_ {
    // self public object
    Condition self;

    // constructor data
    String* name;

    // private data
    void* memory;
};

// vtable
Condition_VTable* condition_vtable;

// link methods
int condition_wait(struct Condition* self, uint_64 timeout);
int condition_signal(struct Condition* self, int count);

// local methods
void* condition_anonymous_new();
void condition_anonymous_free(void* memory);
void* condition_named_new(char* name);
void condition_named_free(void* memory, char* name);

// implement methods
void* condition_anonymous_new() {
    // alocate mutex and cond
    void* result = heap_alloc(sizeof(pthread_mutex_t) + sizeof(pthread_cond_t));

    // get mutex and cond address
    pthread_mutex_t* mutex = result;
    pthread_cond_t* cond = result + sizeof(pthread_mutex_t);

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
        // not exists, it was created now
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
    pthread_cond_t* cond = result + sizeof(pthread_mutex_t);
    int* connections = result + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // create and init or open and increase connections
    if (!exists) {
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

        // init share connections
        *connections = 1;
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
        // destroy share mutex, cond
        pthread_mutex_destroy(mutex);
        pthread_cond_destroy(cond);

        // unmap share memory
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int));

        // unlink (it has not any connections)
        shm_unlink(name);
    } else {
        // reduce connections
        *connections -= 1;

        // unmap share memory
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int));

        // dont unlink (it has another connections)
    }
}

// vtable operators
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

        // get time_out
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;

        // timed wait
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
    if (count > 0) {
        // signal
        for (int cursor = 0; cursor < count; cursor++) {
            pthread_cond_signal(cond);
        }
        result = 0;
    } else {
        // broadcast
        if (pthread_cond_broadcast(cond) == 0) {
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}

// object allocation and deallocation operators
void condition_init() {
    // init vtable
    condition_vtable = heap_alloc(sizeof(Condition_VTable));
    condition_vtable->wait = condition_wait;
    condition_vtable->signal = condition_signal;
}
Condition* condition_new() {
    struct Condition_* condition_ = heap_alloc(sizeof(struct Condition_));

    // set vtable
    condition_->self.vtable = condition_vtable;

    // set constructor data
    condition_->name = NULL;

    // set private data
    condition_->memory = NULL;

    return (Condition*)condition_;
}
void condition_free(Condition* condition) {
    struct Condition_* condition_ = (struct Condition_*)condition;

    // free private data
    if (condition_->memory != NULL) {
        if (condition_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // destroy and close or close internal share mutex and cond
            condition_named_free(condition_->memory, condition_->name->vtable->value(condition_->name));

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal mutex and cond
            condition_anonymous_free(condition_->memory);
        }
    }

    // free constructor data
    if (condition_->name != NULL) {
        string_free(condition_->name);
    }

    // free self
    heap_free(condition_);
}
Condition* condition_new_object(char* name) {
    struct Condition_* condition_ = (struct Condition_*)condition_new();

    // set constructor data
    if (name != NULL) {
        condition_->name = string_new_printf("%s_condition", name);
    }

    // set private data
    if (name != NULL) {
        // try acquire critical mutex
        if (critical != NULL) {
            critical->vtable->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share mutex
        condition_->memory = condition_named_new(condition_->name->vtable->value(condition_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->vtable->release(critical);
        }
    } else {
        // create internal mutex
        condition_->memory = condition_anonymous_new();
    }

    return (Condition*)condition_;
}

#endif