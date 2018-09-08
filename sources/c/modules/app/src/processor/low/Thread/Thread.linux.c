#include <processor/low/Thread.h>

#if defined(APP_LINUX)

#include <memory/low/Heap.h>
#include <pthread.h>

struct Thread_ {
    struct Thread self;
    pthread_t id;
    tsize stack;
};

// link methods
int thread_priority(struct Thread* self, int priority);
int thread_affinity(struct Thread* self, int affinity);
int thread_start(struct Thread* self, void* (*function)(void*), void* arg);
int thread_join(struct Thread* self);
int thread_id(struct Thread* self);
int thread_stop(struct Thread* self);

// implement methods
int thread_priority(struct Thread* self, int priority) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // set priority
    int result = -1;

    return result;
}
int thread_affinity(struct Thread* self, int affinity) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // set affinity
    int result = -1;

    return result;
}
int thread_start(struct Thread* self, void* (*function)(void*), void* arg) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // start internal pthread
    int result = -1;
    pthread_attr_t tattr;
    pthread_attr_init(&tattr);
    if (thread_->stack != -1) {
        pthread_attr_setstacksize(&tattr, thread_->stack);
    }
    if (pthread_create(&(thread_->id), &tattr, function, arg) == 0) {
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

    // get internal pthread id
    int result = (int)thread_->id;

    return result;
}
int thread_stop(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // stop internal pthread
    int result = pthread_cancel(thread_->id);

    return result;
}

Thread* thread_new(tsize stack) {
    struct Thread_* thread_ = memory_alloc(sizeof(struct Thread_));

    // init private methods
    thread_->self.priority = thread_priority;
    thread_->self.affinity = thread_affinity;
    thread_->self.start = thread_start;
    thread_->self.join = thread_join;
    thread_->self.id = thread_id;
    thread_->self.stop = thread_stop;

    // set stack size
    thread_->stack = stack;

    return (Thread*)thread_;
}
void thread_free(Thread* thread) {
    struct Thread_* thread_ = (struct Thread_*)thread;

    heap_free(thread_);
}

#endif