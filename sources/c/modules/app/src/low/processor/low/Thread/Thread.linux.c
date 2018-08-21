#ifdef __unix__

#include <low/processor/low/Thread.h>

#include <pthread.h>

struct Thread_ {
    struct Thread self;
    pthread_t id;
};

// link methods
int thread_start(struct Thread* self, void* (*function)(void* arg), void* arg);
void* thread_join(struct Thread* self);
int thread_id(struct Thread* self);
int thread_stop(struct Thread* self);

int thread_start(struct Thread* self, void* (*function)(void* arg), void* arg) {
    struct Thread_* thread_ = self;

    // start internal pthread
    pthread_create(&(thread_->id), NULL, function, arg);

    return 0;
}
void* thread_join(struct Thread* self) {
    struct Thread_* thread_ = self;

    // join internal pthread
    void* result = NULL;
    pthread_join(&(thread_->id), &result);

    return result;
}
int thread_id(struct Thread* self) {
    struct Thread_* thread_ = self;

    // get internal pthread id
    int result = thread_->id;

    return result;
}
int thread_stop(struct Thread* self) {
    struct Thread_* thread_ = self;

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

    // init internal pthread

    return thread_;
}
void thread_free(struct Thread* thread) {
    struct Thread_* thread_ = thread;

    // destroy internal pthread

    memory_free(thread_);
}

#endif