#include <low/Thread.h>

#if defined(APP_WINDOWS) && defined(APP_WINDOWS_MINGW)

#include <low/Heap.h>

struct Thread_ {
    // self public object
    Thread self;

    // constructor data
    tsize stack;

    // private data
    HANDLE thread_handle;
    DWORD thread_id;

};

struct Thread_Arg{
    int (*function)(void* arg);
    void* arg;
};

// vtable
Thread_VTable* thread_vtable;

// link methods
int thread_start(Thread* self, int (*function)(void*), void* arg);
int thread_join(Thread* self);
uint_64 thread_id(Thread* self);
int thread_stop(Thread* self);

// local methods
DWORD WINAPI thread_function(LPVOID arg);

// implement methods
DWORD WINAPI thread_function(LPVOID arg){
    struct Thread_Arg* thread_arg = arg;

    DWORD result = (DWORD) thread_arg->function(thread_arg->arg);

    heap_free(thread_arg);

    return result;
}

// vtable operators
int thread_start(Thread* self, int (*function)(void*), void* arg) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // create thread arg
    struct Thread_Arg* thread_arg = heap_alloc(sizeof(struct Thread_Arg));
    thread_arg->function = function;
    thread_arg->arg = arg;

    // start internal thread
    thread_->thread_handle = CreateThread(
            NULL,
            thread_->stack,
            thread_function,
            thread_arg,
            0,
            &thread_->thread_id);
    if (thread_->thread_handle != INVALID_HANDLE_VALUE) {
        return 0;
    }

    return -1;
}
int thread_join(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // join internal thread
    // wait for thread to exit
    if (WaitForSingleObject(thread_->thread_handle, INFINITE) != WAIT_OBJECT_0) {
        return -1;
    }

    // get thread exit code
    int result = -1;
    if (GetExitCodeThread(thread_->thread_handle, (PDWORD) &result) == 0) {
        result = -1;
    }

    return result;
}
uint_64 thread_id(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // get internal thread id
    uint_64 result = thread_->thread_id;

    return result;
}
int thread_stop(Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // stop internal thread
    // terminate thread
    if (TerminateThread(thread_->thread_handle, (DWORD) -1) != 0) {
        return 0;
    }

    return -1;
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
    thread_->thread_handle = INVALID_HANDLE_VALUE;

    return (Thread*)thread_;
}
void thread_free(Thread* thread) {
    struct Thread_* thread_ = (struct Thread_*)thread;

    // free private data
    if(thread_->thread_handle != INVALID_HANDLE_VALUE){
        CloseHandle(thread_->thread_handle);
    }

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
