#include <low/ErrorCheckLock.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>
#include <low/String.h>

struct ErrorCheckLock_ {
    // self public object
    ErrorCheckLock self;

    // constructor data

    // private data
    HANDLE semaphore_handle;
};

// vtable
ErrorCheckLock_VTable* errorchecklock_vtable;

// link methods
int errorchecklock_lock(ErrorCheckLock* self, uint_64 timeout);
int errorchecklock_unlock(ErrorCheckLock* self);

// implement methods
// vtable operators
int errorchecklock_lock(ErrorCheckLock* self, uint_64 timeout) {
    struct ErrorCheckLock_* errorchecklock_ = (struct ErrorCheckLock_*)self;

    // wait the wi32 semaphore
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(errorchecklock_->semaphore_handle, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed, try
        if (WaitForSingleObject(errorchecklock_->semaphore_handle, (DWORD) timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int errorchecklock_unlock(ErrorCheckLock* self) {
    struct ErrorCheckLock_* errorchecklock_ = (struct ErrorCheckLock_*)self;

    // release the win32 semaphore (check error)
    if (ReleaseSemaphore(errorchecklock_->semaphore_handle, 1, NULL) != 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void errorchecklock_init() {
    // init vtable
    errorchecklock_vtable = heap_alloc(sizeof(ErrorCheckLock_VTable));
    errorchecklock_vtable->lock = errorchecklock_lock;
    errorchecklock_vtable->unlock = errorchecklock_unlock;
}
ErrorCheckLock* errorchecklock_new() {
    struct ErrorCheckLock_* errorchecklock_ = heap_alloc(sizeof(struct ErrorCheckLock_));

    // set vtable
    errorchecklock_->self.vtable = errorchecklock_vtable;

    // set constructor data

    // set private data
    errorchecklock_->semaphore_handle = INVALID_HANDLE_VALUE;

    return (ErrorCheckLock*)errorchecklock_;
}
void errorchecklock_free(ErrorCheckLock* errorchecklock) {
    struct ErrorCheckLock_* errorchecklock_ = (struct ErrorCheckLock_*)errorchecklock;

    // free private data
    if (errorchecklock_->semaphore_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(errorchecklock_->semaphore_handle);
    }

    // free constructor data

    // free self
    heap_free(errorchecklock_);
}
ErrorCheckLock* errorchecklock_new_anonymous(){
    struct ErrorCheckLock_* errorchecklock_ = (struct ErrorCheckLock_*)errorchecklock_new();

    // set constructor data

    // set private data
    errorchecklock_->semaphore_handle = CreateSemaphoreA(
            NULL,
            1,
            1,
            NULL);

    return (ErrorCheckLock*)errorchecklock_;
}
ErrorCheckLock* errorchecklock_new_named(char* name){
    struct ErrorCheckLock_* errorchecklock_ = (struct ErrorCheckLock_*)errorchecklock_new();

    // set constructor data


    // set private data
    String* errorchecklock_name = string_new_printf("%s_el", name);
    errorchecklock_->semaphore_handle = CreateSemaphoreA(
            NULL,
            1,
            1,
            errorchecklock_name->vtable->value(errorchecklock_name));
    string_free(errorchecklock_name);

    return (ErrorCheckLock*)errorchecklock_;
}

#endif
