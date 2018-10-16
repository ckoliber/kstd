#include <low/Thread.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>

struct Thread_ {
    // self public object
    Thread self;

    // constructor data
    tsize stack;

    // private data
    int id;
};

// vtable
Thread_VTable* thread_vtable;

// link methods
int thread_start(Thread* self, int (*function)(void*), void* arg);
int thread_join(Thread* self);
int thread_id(Thread* self);
int thread_stop(Thread* self);

// implement methods
// vtable operators
int thread_start(Thread* self, int (*function)(void*), void* arg) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // start internal thread
    HANDLE thread = CreateThread(
        NULL,
        thread_->stack,
        function,
        arg,
        0,
        &thread_->id);
    if (thread != INVALID_HANDLE_VALUE) {
        CloseHandle(thread);
        return 0;
    }

    return -1;
}
int thread_join(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // join internal thread
    // open thread by id
    HANDLE thread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_->id);
    if (thread == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // wait for thread to exit
    if (WaitForSingleObject(thread, INFINITE) != WAIT_OBJECT_0) {
        CloseHandle(thread);
        return -1;
    }

    // get thread exit code
    int result = -1;
    if (GetExitCodeThread(thread, &result) == 0) {
        result = -1;
    }

    // close thread handle
    CloseHandle(thread);

    return result;
}
int thread_id(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // get internal thread id
    int result = thread_->id;

    return result;
}
int thread_stop(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // stop internal thread
    // open thread by id
    HANDLE thread = OpenThread(THREAD_ALL_ACCESS, FALSE, thread_->id);
    if (thread == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // terminate thread
    if (TerminateThread(thread, -1) == 0) {
        CloseHandle(thread);
        return -1;
    }

    // close thread handle
    CloseHandle(thread);

    return 0;
}

// object allocation and deallocation operators
void thread_init() {
    // init vtable
    thread_vtable = heap_alloc(sizeof(Thread_VTable));
    thread_vtable->start = thread_start;
    thread_vtable->join = thread_join;
    thread_vtable->id = thread_id;
    thread_vtable->stop = thread_stop;
}
Thread* thread_new() {
    struct Thread_* thread_ = heap_alloc(sizeof(struct Thread_));

    // set vtable
    thread_->self.vtable = thread_vtable;

    // set constructor data
    thread_->stack = 0;

    // set private data
    thread_->id = 0;

    return (Thread*)thread_;
}
void thread_free(Thread* thread) {
    struct Thread_* thread_ = (struct Thread_*)thread;

    // free private data

    // free self
    heap_free(thread_);
}
Thread* thread_new_object(tsize stack) {
    struct Thread_* thread_ = (struct Thread_*)thread_new();

    // set constructor data
    thread_->stack = stack;

    // set private data

    return (Thread*)thread_;
}

// local thread methods
int thread_get_self() {
    // get self thread id
    int result = (int)GetCurrentThreadId();

    return result;
}

#endif