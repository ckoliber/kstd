#include <low/MutexLock.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>
#include <low/String.h>

struct MutexLock_ {
    // self public object
    MutexLock self;

    // constructor data

    // private data
    HANDLE semaphore_handle;
};

// vtable
MutexLock_VTable* mutexlock_vtable;

// link methods
int mutexlock_lock(MutexLock* self, uint_64 timeout);
int mutexlock_unlock(MutexLock* self);

// implement methods
// vtable operators
int mutexlock_lock(MutexLock* self, uint_64 timeout) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)self;

    // wait the wi32 semaphore
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(mutexlock_->semaphore_handle, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed, try
        if (WaitForSingleObject(mutexlock_->semaphore_handle, (DWORD) timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int mutexlock_unlock(MutexLock* self) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)self;

    // release the win32 semaphore (not check error)
    if (ReleaseSemaphore(mutexlock_->semaphore_handle, 1, NULL) != 0) {
        return 0;
    }

    return 0;
}

// object allocation and deallocation operators
void mutexlock_init() {
    // init vtable
    mutexlock_vtable = heap_alloc(sizeof(MutexLock_VTable));
    mutexlock_vtable->lock = mutexlock_lock;
    mutexlock_vtable->unlock = mutexlock_unlock;
}
MutexLock* mutexlock_new() {
    struct MutexLock_* mutexlock_ = heap_alloc(sizeof(struct MutexLock_));

    // set vtable
    mutexlock_->self.vtable = mutexlock_vtable;

    // set constructor data

    // set private data
    mutexlock_->semaphore_handle = INVALID_HANDLE_VALUE;

    return (MutexLock*)mutexlock_;
}
void mutexlock_free(MutexLock* mutexlock) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)mutexlock;

    // free private data
    if (mutexlock_->semaphore_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(mutexlock_->semaphore_handle);
    }

    // free constructor data

    // free self
    heap_free(mutexlock_);
}
MutexLock* mutexlock_new_anonymous(){
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)mutexlock_new();

    // set constructor data

    // set private data
    mutexlock_->semaphore_handle = CreateSemaphoreA(
            NULL,
            1,
            1,
            NULL);

    return (MutexLock*)mutexlock_;
}
MutexLock* mutexlock_new_named(char* name){
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)mutexlock_new();

    // set constructor data

    // set private data
    String* mutexlock_name = string_new_printf("%s_ml", name);
    mutexlock_->semaphore_handle = CreateSemaphoreA(
            NULL,
            1,
            1,
            mutexlock_name->vtable->value(mutexlock_name));
    string_free(mutexlock_name);

    return (MutexLock*)mutexlock_;
}

#endif
