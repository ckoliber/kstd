#include <low/processor/high/ThreadPool.h>

#include <io/memory/Memory.h>
#include <low/processor/low/Thread.h>

struct ThreadPool_ {
    struct ThreadPool self;
};

// link methods
int threadpool_start(struct ThreadPool* self);
int threadpool_post(struct ThreadPool* self, void (*function)(void* arg), void* arg);
int threadpool_stop(struct ThreadPool* self);

int threadpool_start(struct ThreadPool* self) {}
int threadpool_post(struct ThreadPool* self, void (*function)(void* arg), void* arg) {}
int threadpool_stop(struct ThreadPool* self) {}

struct ThreadPool* threadpool_new(int size) {
    struct ThreadPool_* threadpool_ = memory_alloc(sizeof(struct ThreadPool_));

    // init private methods
    threadpool_->self.start = threadpool_start;
    threadpool_->self.post = threadpool_post;
    threadpool_->self.stop = threadpool_stop;

    // init internal Threads

    return threadpool_;
}
void threadpool_free(struct ThreadPool* threadpool) {
    struct ThreadPool_* threadpool_ = threadpool;

    // destroy internal Threads

    memory_free(threadpool_);
}