#include <low/Semaphore.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/String.h>

struct Semaphore_ {
    // self public object
    Semaphore self;

    // constructor data
    String* name;

    // private data
    HANDLE semaphore;
};

// vtable
Semaphore_VTable* semaphore_vtable;

// link methods
int semaphore_init(struct Semaphore* self, int value);
int semaphore_wait(struct Semaphore* self, uint_64 timeout);
int semaphore_post(struct Semaphore* self);
int semaphore_get(struct Semaphore* self);

// implement methods
// vtable operators
int semaphore_init(struct Semaphore* self, int value) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // reopen semaphore using new value
    CloseHandle(semaphore_->semaphore);
    semaphore_->semaphore = CreateSemaphoreA(NULL, value, UINT_32_MAX, semaphore_->name->vtable->value(semaphore_->name));
    if (semaphore_->semaphore != INVALID_HANDLE_VALUE) {
        return 0;
    }

    return -1;
}
int semaphore_wait(struct Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // wait the win32 semaphore
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(semaphore_->semaphore, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed
        if (WaitForSingleObject(semaphore_->semaphore, timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int semaphore_post(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // post the win32 semaphore
    if (ReleaseSemaphore(semaphore_->semaphore, 1, NULL) != 0) {
        return 0;
    }

    return -1;
}
int semaphore_get(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get the win32 semaphore value
    int result = 0;
    if (ReleaseSemaphore(semaphore_->semaphore, 0, &result) == 0) {
        result = -1;
    }

    return result;
}

// object allocation and deallocation operators
void semaphore_init() {
    // init vtable
    semaphore_vtable = heap_alloc(sizeof(Semaphore_VTable));
    semaphore_vtable->init = semaphore_init;
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
    semaphore_->semaphore = INVALID_HANDLE_VALUE;

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    // free private data
    if (semaphore_->semaphore != INVALID_HANDLE_VALUE) {
        CloseHandle(semaphore_->semaphore);
    }

    // free constructor data
    if (semaphore_->name != NULL) {
        string_free(semaphore_->name);
    }

    // free self
    heap_free(semaphore_);
}
Semaphore* semaphore_new_object(char* name) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data
    if (name != NULL) {
        semaphore_->name = string_new_printf("%s_semaphore", name);
    }

    // set private data
    semaphore_->semaphore = CreateSemaphoreA(NULL, 0, UINT_32_MAX, name);

    return (Semaphore*)semaphore_;
}

#endif