#include <low/Thread.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <low/Heap.h>
#include <pthread.h>

struct Thread_ {
    // self public object
    Thread self;

    // constructor data
    tsize stack;

    // private data
    pthread_t id;
};

// vtable
Thread_VTable* thread_vtable;

// link methods
int thread_start(struct Thread* self, int (*function)(void*), void* arg);
int thread_join(struct Thread* self);
int thread_id(struct Thread* self);
int thread_stop(struct Thread* self);

// implement methods
// vtable operators
int thread_start(struct Thread* self, int (*function)(void*), void* arg) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // start internal pthread
    int result = -1;
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);
    if (thread_->stack > 0) {
        pthread_attr_setstacksize(&tattr, thread_->stack);
    }
    if (pthread_create(&thread_->id, &tattr, function, arg) == 0) {
        result = 0;
    }
    pthread_attr_destroy(&tattr);

    return result;
}
int thread_join(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // join internal pthread
    int result = -1;
    pthread_join(thread_->id, &result);

    return result;
}
int thread_id(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // get internal thread id
    int result = (int)thread_->id;

    return result;
}
int thread_stop(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // stop internal pthread
    if (pthread_cancel(thread_->id) == 0) {
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

// local string methods
int thread_self() {
    // get self thread id
    int result = (int)pthread_self();

    return result;
}

#endif