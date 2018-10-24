#include <low/Semaphore.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>
#include <low/String.h>

struct Semaphore_ {
    // self public object
    Semaphore self;

    // constructor data
    String* name;

    // private data
    HANDLE semaphore_handle;
};

// vtable
Semaphore_VTable* semaphore_vtable;

// link methods
int semaphore_wait(Semaphore* self, uint_64 timeout);
int semaphore_post(Semaphore* self);
int semaphore_get(Semaphore* self);

// implement methods
// vtable operators
int semaphore_wait(Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // wait the wi32 semaphore
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(semaphore_->semaphore_handle, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed, try
        if (WaitForSingleObject(semaphore_->semaphore_handle, (DWORD) timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int semaphore_post(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // release the win32 semaphore
    if (ReleaseSemaphore(semaphore_->semaphore_handle, 1, NULL) != 0) {
        return 0;
    }

    return -1;
}
int semaphore_get(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get the win32 semaphore value
    int result = 0;
    if (ReleaseSemaphore(semaphore_->semaphore_handle, 0, (LPLONG) &result) == 0) {
        result = -1;
    }

    return result;
}

// object allocation and deallocation operators
void semaphore_init() {
    // init vtable
    semaphore_vtable = (Semaphore_VTable*)heap_alloc(sizeof(Semaphore_VTable));
    semaphore_vtable->wait = semaphore_wait;
    semaphore_vtable->post = semaphore_post;
    semaphore_vtable->get = semaphore_get;
}
Semaphore* semaphore_new() {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)heap_alloc(sizeof(struct Semaphore_));

    // set vtable
    semaphore_->self.vtable = semaphore_vtable;

    // set constructor data

    // set private data
    semaphore_->semaphore_handle = INVALID_HANDLE_VALUE;

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    // free private data
    if (semaphore_->semaphore_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(semaphore_->semaphore_handle);
    }

    // free constructor data
    if (semaphore_->name != NULL) {
        string_free(semaphore_->name);
    }

    // free self
    heap_free((uint_8*)semaphore_);
}
Semaphore* semaphore_new_object(char* name, int value) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data
    if (name != NULL) {
        semaphore_->name = string_new_printf("%s_sm", name);
    }

    // set private data
    if(name != NULL){
        semaphore_->semaphore_handle = CreateSemaphoreA(
                NULL,
                value,
                UINT_32_MAX,
                semaphore_->name->vtable->value(semaphore_->name));
    }else{
        semaphore_->semaphore_handle = CreateSemaphoreA(
                NULL,
                value,
                UINT_32_MAX,
                NULL);
    }

    return (Semaphore*)semaphore_;
}

#endif
