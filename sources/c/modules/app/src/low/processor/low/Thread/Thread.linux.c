#ifdef __unix__

#include <low/processor/low/Thread.h>

#include <io/memory/Memory.h>
#include <pthread.h>

struct Thread_ {
    struct Thread self;
    pthread_t id;
};

// link methods
int thread_start(struct Thread* self, void* (*function)(void*), void* arg);
void* thread_join(struct Thread* self);
int thread_id(struct Thread* self);
int thread_stop(struct Thread* self);

int thread_start(struct Thread* self, void* (*function)(void*), void* arg) {
    struct Thread_* thread_ = (struct Thread_ *) self;

    // start internal pthread
    int result = 0;
    if (pthread_create(&(thread_->id), NULL, function, arg)) {
        result = -1;
    }

    return result;
}
void* thread_join(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_ *) self;

    // join internal pthread
    void* result = NULL;
    if (pthread_join(thread_->id, &result)) {
        result = NULL;
    }

    return result;
}
int thread_id(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_ *) self;

    // get internal pthread id
    int result = (int) thread_->id;

    return result;
}
int thread_stop(struct Thread* self) {
    struct Thread_* thread_ = (struct Thread_ *) self;

    // stop internal pthread
    pthread_cancel(thread_->id);

    return 0;
}

struct Thread* thread_new() {
    struct Thread_* thread_ = memory_alloc(sizeof(struct Thread_));

    // init private methods
    thread_->self.start = thread_start;
    thread_->self.join = thread_join;
    thread_->self.id = thread_id;
    thread_->self.stop = thread_stop;

    return (struct Thread *) thread_;
}
void thread_free(struct Thread* thread) {
    struct Thread_* thread_ = (struct Thread_ *) thread;

    memory_free(thread_);
}

#endif