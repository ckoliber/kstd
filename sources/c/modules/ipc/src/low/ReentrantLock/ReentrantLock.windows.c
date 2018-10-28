#include <low/ReentrantLock.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>
#include <low/String.h>

struct ReentrantLock_ {
    // self public object
    ReentrantLock self;

    // constructor data

    // private data
    HANDLE mutex_handle;
};

// vtable
ReentrantLock_VTable* reentrantlock_vtable;

// link methods
int reentrantlock_lock(ReentrantLock* self, uint_64 timeout);
int reentrantlock_unlock(ReentrantLock* self);

// implement methods
// vtable operators
int reentrantlock_lock(ReentrantLock* self, uint_64 timeout) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)self;

    // wait the wi32 mutex
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(reentrantlock_->mutex_handle, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed, try
        if (WaitForSingleObject(reentrantlock_->mutex_handle, (DWORD) timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int reentrantlock_unlock(ReentrantLock* self) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)self;

    // release the win32 mutex (recursive)
    if (ReleaseMutex(reentrantlock_->mutex_handle) != 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void reentrantlock_init() {
    // init vtable
    reentrantlock_vtable = heap_alloc(sizeof(ReentrantLock_VTable));
    reentrantlock_vtable->lock = reentrantlock_lock;
    reentrantlock_vtable->unlock = reentrantlock_unlock;
}
ReentrantLock* reentrantlock_new() {
    struct ReentrantLock_* reentrantlock_ = heap_alloc(sizeof(struct ReentrantLock_));

    // set vtable
    reentrantlock_->self.vtable = reentrantlock_vtable;

    // set constructor data

    // set private data
    reentrantlock_->mutex_handle = INVALID_HANDLE_VALUE;

    return (ReentrantLock*)reentrantlock_;
}
void reentrantlock_free(ReentrantLock* reentrantlock) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)reentrantlock;

    // free private data
    if (reentrantlock_->mutex_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(reentrantlock_->mutex_handle);
    }

    // free constructor data

    // free self
    heap_free(reentrantlock_);
}
ReentrantLock* reentrantlock_new_anonymous(){
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)reentrantlock_new();

    // set constructor data

    // set private data
    reentrantlock_->mutex_handle = CreateMutexA(
            NULL,
            FALSE,
            NULL);

    return (ReentrantLock*)reentrantlock_;
}
ReentrantLock* reentrantlock_new_named(char* name){
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)reentrantlock_new();

    // set constructor data

    // set private data
    String* reentrantlock_name = string_new_printf("%s_rl", name);
    reentrantlock_->mutex_handle = CreateMutexA(
            NULL,
            FALSE,
            reentrantlock_name->vtable->value(reentrantlock_name));
    string_free(reentrantlock_name);

    return (ReentrantLock*)reentrantlock_;
}

#endif
