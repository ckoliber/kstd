#include <ipc/low/Condition.h>

#if defined(APP_ANDROID)

#include <dsa/low/String.h>
#include <ipc/low/Mutex.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <pthread.h>

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
    // android does not support posix share memory
    return NULL;
}
void condition_named_free(void* memory, char* name) {
    // android does not support posix share memory
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
            // android does not support posix share memory
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
        condition_->name = string_new_concat(name, "/condition");
    }

    // set private data
    if (name != NULL) {
        // android does not support posix share memory
        condition_free(condition_);
        condition_ = NULL;
    } else {
        // create internal mutex
        condition_->memory = mutex_anonymous_new();
    }

    return (Condition*)condition_;
}

#endif