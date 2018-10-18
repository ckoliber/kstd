#include <low/Thread.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>

struct Thread_ {
    // self public object
    Thread self;

    // constructor data
    tsize stack;

    // private data
    uint_64 id;
};

struct Thread_Arg{
    int (*function)(uint_8* arg);
    uint_8* arg;
};

// vtable
Thread_VTable* thread_vtable;

// link methods
int thread_start(Thread* self, int (*function)(uint_8*), uint_8* arg);
int thread_join(Thread* self);
uint_64 thread_id(Thread* self);
int thread_stop(Thread* self);

// local methods
DWORD WINAPI thread_function(LPVOID arg);

// implement methods
DWORD WINAPI thread_function(LPVOID arg){
    struct Thread_Arg* thread_arg = arg;

    DWORD result = (DWORD) thread_arg->function(thread_arg->arg);

    heap_free((uint_8*) thread_arg);

    return result;
}

// vtable operators
int thread_start(Thread* self, int (*function)(uint_8*), uint_8* arg) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // create thread arg
    struct Thread_Arg* thread_arg = (struct Thread_Arg*) heap_alloc(sizeof(struct Thread_Arg));
    thread_arg->function = function;
    thread_arg->arg = arg;

    // start internal thread
    HANDLE thread = CreateThread(
            NULL,
            thread_->stack,
            thread_function,
            thread_arg,
            0,
            (PDWORD) &thread_->id);
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
    HANDLE thread = OpenThread(THREAD_ALL_ACCESS, FALSE, (DWORD) thread_->id);
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
    if (GetExitCodeThread(thread, (PDWORD) &result) == 0) {
        result = -1;
    }

    // close thread handle
    CloseHandle(thread);

    return result;
}
uint_64 thread_id(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // get internal thread id
    uint_64 result = thread_->id;

    return result;
}
int thread_stop(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // stop internal thread
    // open thread by id
    HANDLE thread = OpenThread(THREAD_ALL_ACCESS, FALSE, (DWORD) thread_->id);
    if (thread == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // terminate thread
    if (TerminateThread(thread, (DWORD) -1) == 0) {
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
    thread_vtable = (Thread_VTable*) heap_alloc(sizeof(Thread_VTable));
    thread_vtable->start = thread_start;
    thread_vtable->join = thread_join;
    thread_vtable->id = thread_id;
    thread_vtable->stop = thread_stop;
}
Thread* thread_new() {
    struct Thread_* thread_ = (struct Thread_*) heap_alloc(sizeof(struct Thread_));

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
    heap_free((uint_8*) thread_);
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
