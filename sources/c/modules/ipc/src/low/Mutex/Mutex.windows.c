#include <low/Mutex.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/String.h>

struct Mutex_ {
    // self public object
    Mutex self;

    // constructor data

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

    // set private data
    mutex_->mutex = INVALID_HANDLE_VALUE;

    return (Mutex*)mutex_;
}
void mutex_free(Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    // free private data
    if (mutex_->mutex != INVALID_HANDLE_VALUE) {
        CloseHandle(mutex_->mutex);
    }

    // free constructor data

    // free self
    heap_free(mutex_);
}
Mutex* mutex_new_object(int mode, char* name) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex_new();

    // set constructor data

    // set private data
    mutex_->mutex = CreateMutexA(NULL, FALSE, name);

    return (Mutex*)mutex_;
}

#endif