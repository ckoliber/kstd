#include <low/Mutex.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/String.h>

struct Mutex_ {
    // self public object
    Mutex self;

    // constructor data
    String* name;

    // private data
    HANDLE mutex;
};

// vtable
Mutex_VTable* mutex_vtable;

// link methods
int mutex_acquire(struct Mutex* self, uint_64 timeout);
int mutex_release(struct Mutex* self);

// implement methods
// vtable operators
int mutex_acquire(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // aquire the win32 mutex
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(mutex_->mutex, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed - try
        if (WaitForSingleObject(mutex_->mutex, timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int mutex_release(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // release the win32 mutex
    if (ReleaseMutex(mutex_->mutex) != 0) {
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
Mutex* mutex_new() {
    struct Mutex_* mutex_ = heap_alloc(sizeof(struct Mutex_));

    // set vtable
    mutex_->self.vtable = mutex_vtable;

    // set constructor data
    mutex_->name = NULL;

    // set private data
    mutex_->mutex = INVALID_HANDLE_VALUE;

    return (Mutex*)mutex_;
}
void mutex_free(Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    // free private data
    if (mutex_->mutex != INVALID_HANDLE_VALUE) {
        if (mutex_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // close internal
            CloseHandle(mutex_->mutex);

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal mutex
            CloseHandle(mutex_->mutex);
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
    struct Mutex_* mutex_ = (struct Mutex_*)mutex_new();

    // set constructor data
    if (name != NULL) {
        mutex_->name = string_new_printf("%s_mutex", name);
    }

    // set private data
    switch (mode) {
        case 0:
            mutex_->mutex = CreateMutexA(NULL, FALSE, name);
            break;
        case 1:
            break;
        case 2:
            break;
    }

    return (Mutex*)mutex_;
}

#endif