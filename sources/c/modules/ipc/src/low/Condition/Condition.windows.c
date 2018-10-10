#include <low/Condition.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/String.h>

struct Condition_ {
    // self public object
    Condition self;

    // constructor data

    // private data
    HANDLE mutex;
    HANDLE semaphore;
};

// vtable
Condition_VTable* condition_vtable;

// link methods
int condition_wait(struct Condition* self, uint_64 timeout);
int condition_signal(struct Condition* self, int count);

// implement methods
// vtable operators
int condition_wait(struct Condition* self, uint_64 timeout) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // wait the win32 semaphore
    SignalObjectAndWait();

    return -1;
}
int condition_signal(struct Condition* self, int count) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // wait the win32 semaphore
    if (count > 0) {
        // signal
        for (int cursor = 0; cursor < count; cursor++) {
            pthread_cond_signal(cond);
        }
        return 0;
    } else {
        // broadcast
        if (pthread_cond_broadcast(cond) == 0) {
            return 0;
        }
    }

    return -1;
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

    // set private data
    condition_->mutex = INVALID_HANDLE_VALUE;
    condition_->semaphore = INVALID_HANDLE_VALUE;

    return (Condition*)condition_;
}
void condition_free(Condition* condition) {
    struct Condition_* condition_ = (struct Condition_*)condition;

    // free private data
    if (condition_->mutex != NULL) {
        CloseHandle(condition_->mutex);
    }
    if (condition_->semaphore != NULL) {
        CloseHandle(condition_->semaphore);
    }

    // free constructor data

    // free self
    heap_free(condition_);
}
Condition* condition_new_object(char* name) {
    struct Condition_* condition_ = (struct Condition_*)condition_new();

    // set constructor data

    // set private data
    condition_->mutex = CreateMutexA(NULL, FALSE, name);
    condition_->semaphore = CreateSemaphore(NULL, 0, 1, name);

    return (Condition*)condition_;
}

#endif