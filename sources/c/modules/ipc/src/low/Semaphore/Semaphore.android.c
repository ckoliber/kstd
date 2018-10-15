#include <low/Semaphore.h>

#if defined(APP_ANDROID)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>
#include <errno.h>

struct Semaphore_ {
    // self public object
    Semaphore self;

    // constructor data
    String* name;

    // private data
    void* memory;
};

// vtable
Semaphore_VTable* semaphore_vtable;

// link methods
int semaphore_wait(Semaphore* self, uint_64 timeout);
int semaphore_post(Semaphore* self);
int semaphore_get(Semaphore* self);

// local methods
void* semaphore_anonymous_new(int value);
void semaphore_anonymous_free(void* memory);
void* semaphore_named_new(char* name, int value);
void semaphore_named_free(void* memory, char* name);

// implement methods
void* semaphore_anonymous_new(int value) {
    // alocate mutex and cond and svalue
    void* result = heap_alloc(sizeof(pthread_mutex_t) + sizeof(pthread_cond_t) + sizeof(int));

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = result;
    pthread_cond_t* cond = result + sizeof(pthread_mutex_t);
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
    *svalue = value;

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
void* semaphore_named_new(char* name, int value) {
    // android does not implement standard share memory
    return NULL;
}
void semaphore_named_free(void* memory, char* name) {
    // android does not implement standard share memory
}

// vtable operators
int semaphore_wait(Semaphore* self, uint_64 timeout) {
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
        (*svalue)--;
        result = 0;
    } else {
        // timed

        // get time_out
        bool timedout = false;
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;

        // timed wait
        while (*svalue == 0) {
            if (pthread_cond_timedwait(cond, mutex, &time_out) == ETIMEDOUT) {
                timedout = true;
                break;
            }
        }

        // check timeouted
        if (!timedout) {
            (*svalue)--;
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}
int semaphore_post(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get mutex and cond and svalue address
    pthread_mutex_t* mutex = semaphore_->memory;
    pthread_cond_t* cond = semaphore_->memory + sizeof(pthread_mutex_t);
    int* svalue = semaphore_->memory + sizeof(pthread_mutex_t) + sizeof(pthread_cond_t);

    // aquire the pthread mutex
    pthread_mutex_lock(mutex);

    // signal the pthread cond
    int result = -1;
    (*svalue)++;
    if (pthread_cond_signal(cond) == 0) {
        result = 0;
    }

    // release the pthread mutex
    pthread_mutex_unlock(mutex);

    return result;
}
int semaphore_get(Semaphore* self) {
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

// object allocation and deallocation operators
void semaphore_init() {
    // init vtable
    semaphore_vtable = heap_alloc(sizeof(Semaphore_VTable));
    semaphore_vtable->wait = semaphore_wait;
    semaphore_vtable->post = semaphore_post;
    semaphore_vtable->get = semaphore_get;
}
Semaphore* semaphore_new() {
    struct Semaphore_* semaphore_ = heap_alloc(sizeof(struct Semaphore_));

    // set vtable
    semaphore_->self.vtable = semaphore_vtable;

    // set constructor data
    semaphore_->name = NULL;

    // set private data
    semaphore_->memory = NULL;

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    // free private data
    if (semaphore_->memory != NULL) {
        if (semaphore_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // destroy and close or close internal share mutex and cond and svalue
            semaphore_named_free(semaphore_->memory, semaphore_->name->vtable->value(semaphore_->name));

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal mutex and cond and svalue
            semaphore_anonymous_free(semaphore_->memory);
        }
    }

    // free constructor data
    if (semaphore_->name != NULL) {
        string_free(semaphore_->name);
    }

    // free self
    heap_free(semaphore_);
}
Semaphore* semaphore_new_object(char* name, int value) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data
    if (name != NULL) {
        semaphore_->name = string_new_printf("%s_semaphore", name);
    }

    // set private data
    if (name != NULL) {
        // try acquire critical mutex
        if (critical != NULL) {
            critical->vtable->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share mutex and cond and svalue
        semaphore_->memory = semaphore_named_new(semaphore_->name->vtable->value(semaphore_->name), value);

        // try release critical mutex
        if (critical != NULL) {
            critical->vtable->release(critical);
        }
    } else {
        // create internal mutex and cond and svalue
        semaphore_->memory = semaphore_anonymous_new(value);
    }

    return (Semaphore*)semaphore_;
}

#endif