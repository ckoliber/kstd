#include <low/Condition.h>

#if defined(APP_ANDROID)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>

struct Condition_Memory{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int connections;
};

struct Condition_ {
    // self public object
    Condition self;

    // constructor data
    String* name;

    // private data
    struct Condition_Memory* memory;
};

// vtable
Condition_VTable* condition_vtable;

// link methods
int condition_wait(Condition* self, uint_64 timeout);
int condition_signal(Condition* self, int count);

// local methods
struct Condition_Memory* condition_anonymous_new();
void condition_anonymous_free(struct Condition_Memory* memory);
struct Condition_Memory* condition_named_new(char* name);
void condition_named_free(struct Condition_Memory* memory, char* name);

// implement methods
struct Condition_Memory* condition_anonymous_new() {
    // alocate mutex and cond
    struct Condition_Memory* result = heap_alloc(sizeof(struct Condition_Memory));

    // init mutex
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 0);
    pthread_mutex_init(&(result->mutex), &mattr);
    pthread_mutexattr_destroy(&mattr);

    // init cond
    pthread_condattr_t cattr;
    pthread_condattr_init(&cattr);
    pthread_condattr_setpshared(&cattr, 0);
    pthread_cond_init(&(result->cond), &cattr);
    pthread_condattr_destroy(&cattr);

    return result;
}
void condition_anonymous_free(struct Condition_Memory* memory) {
    // destroy internal mutex
    pthread_mutex_destroy(&(memory->mutex));
    pthread_cond_destroy(&(memory->cond));

    heap_free(memory);
}
struct Condition_Memory* condition_named_new(char* name) {
    // android does not implement standard share memory
    return condition_anonymous_new();
}
void condition_named_free(struct Condition_Memory* memory, char* name) {
    // android does not implement standard share memory
    condition_anonymous_free(memory);
}

// vtable operators
int condition_wait(Condition* self, uint_64 timeout) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(condition_->memory->mutex));

    // wait the pthread cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_cond_wait(&(condition_->memory->cond), &(condition_->memory->mutex)) == 0) {
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
        if (pthread_cond_timedwait(&(condition_->memory->cond), &(condition_->memory->mutex), &time_out) == 0) {
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(&(condition_->memory->mutex));

    return result;
}
int condition_signal(Condition* self, int count) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(condition_->memory->mutex));

    // signal the pthread cond
    int result = -1;
    if (count > 0) {
        // signal
        for (int cursor = 0; cursor < count; cursor++) {
            pthread_cond_signal(&(condition_->memory->cond));
        }
        result = 0;
    } else {
        // broadcast
        if (pthread_cond_broadcast(&(condition_->memory->cond)) == 0) {
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(&(condition_->memory->mutex));

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