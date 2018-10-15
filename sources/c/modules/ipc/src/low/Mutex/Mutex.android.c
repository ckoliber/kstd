#include <low/Mutex.h>

#if defined(APP_ANDROID)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>

struct Mutex_ {
    // self public object
    Mutex self;

    // constructor data
    String* name;

    // private data
    void* memory;
};

// vtable
Mutex_VTable* mutex_vtable;

// link methods
int mutex_acquire(Mutex* self, uint_64 timeout);
int mutex_release(Mutex* self);

// local methods
void* mutex_anonymous_new(int mode);
void mutex_anonymous_free(void* memory);
void* mutex_named_new(int mode, char* name);
void mutex_named_free(void* memory, char* name);

// implement methods
void* mutex_anonymous_new(int mode) {
    // alocate mutex
    void* result = heap_alloc(sizeof(pthread_mutex_t));

    // get mutex address
    pthread_mutex_t* mutex = result;

    // init mutex
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 0);

    // set mutex type
    switch (mode) {
        case 0:
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
            break;
        case 1:
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
            break;
        case 2:
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_ERRORCHECK);
            break;
    }

    pthread_mutex_init(mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);

    return result;
}
void mutex_anonymous_free(void* memory) {
    // get mutex address
    pthread_mutex_t* mutex = memory;

    // destroy internal mutex
    pthread_mutex_destroy(mutex);

    heap_free(memory);
}
void* mutex_named_new(int mode, char* name) {
    // android does not implement standard share memory
    return NULL;
}
void mutex_named_free(void* memory, char* name) {
    // android does not implement standard share memory
}

// vtable operators
int mutex_acquire(Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // get mutex address
    pthread_mutex_t* mutex = mutex_->memory;

    // aquire the pthread mutex
    if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_mutex_lock(mutex) == 0) {
            return 0;
        }
    } else {
        // timed
        uint_64 time = date_get_epoch();

        // try lock until timeout
        do {
            // try
            if (pthread_mutex_trylock(mutex) == 0) {
                return 0;
            }
        } while ((date_get_epoch() - time) <= timeout);
    }

    return -1;
}
int mutex_release(Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // get mutex address
    pthread_mutex_t* mutex = mutex_->memory;

    // release the pthread mutex
    if (pthread_mutex_unlock(mutex) == 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void mutex_init() {
    // init vtable
    mutex_vtable = heap_alloc(sizeof(Mutex_VTable));
    mutex_vtable->acquire = mutex_acquire;
    mutex_vtable->release = mutex_release;
}
Mutex* mutex_new(int mode) {
    struct Mutex_* mutex_ = heap_alloc(sizeof(struct Mutex_));

    // set vtable
    mutex_->self.vtable = mutex_vtable;

    // set constructor data
    mutex_->name = NULL;

    // set private data
    mutex_->memory = NULL;

    return (Mutex*)mutex_;
}
void mutex_free(Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    // free private data
    if (mutex_->memory != NULL) {
        if (mutex_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // destroy and close or close internal share mutex
            mutex_named_free(mutex_->memory, mutex_->name->vtable->value(mutex_->name));

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal mutex
            mutex_anonymous_free(mutex_->memory);
        }
    }

    // free constructor data
    if (mutex_->name != NULL) {
        string_free(mutex_->name);
    }

    // free self
    heap_free(mutex_);
}
Mutex* mutex_new_object(int mode, char* name) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex_new(mode);

    // set constructor data
    if (name != NULL) {
        mutex_->name = string_new_printf("%s_mutex", name);
    }

    // set private data
    if (name != NULL) {
        // try acquire critical mutex
        if (critical != NULL) {
            critical->vtable->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share mutex
        mutex_->memory = mutex_named_new(mode, mutex_->name->vtable->value(mutex_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->vtable->release(critical);
        }
    } else {
        // create internal mutex
        mutex_->memory = mutex_anonymous_new(mode);
    }

    return (Mutex*)mutex_;
}

#endif