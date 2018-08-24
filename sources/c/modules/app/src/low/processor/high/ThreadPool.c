#include <low/processor/high/ThreadPool.h>

#include <io/memory/Memory.h>
#include <low/dsa/Queue.h>
#include <low/itc/high/Semaphore.h>
#include <low/processor/low/Thread.h>

struct ThreadPool_ {
    struct ThreadPool self;
    struct Semaphore* semaphore;
    struct Queue* queue;
    struct Thread** pool;
    int* signal;
    int size;
};

struct ThreadItem{
    void (*function)(void* arg);
    void* arg;
};

struct ThreadArg{
    struct ThreadPool_* pool;
    int index;
};

// link methods
int threadpool_start(struct ThreadPool* self);
int threadpool_post(struct ThreadPool* self, void (*function)(void*), void* arg);
int threadpool_stop(struct ThreadPool* self, int force);

// local methods
void* thread_loop(void* arg){
    struct ThreadArg* threadarg = arg;

    // thread loop
    while(threadarg->pool->signal[threadarg->index]){
        struct ThreadItem* item = threadarg->pool->queue->dequeue(threadarg->pool->queue, -1);
        item->function(item->arg);
        memory_free(item);
    }

    // signal exit semaphore
    threadarg->pool->semaphore->post(threadarg->pool->semaphore, 1);

    // free thread argument
    memory_free(threadarg);

    return NULL;
}

int threadpool_start(struct ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_ *) self;

    // start all of the threads
    int result = 0;
    for(int cursor = 0 ; cursor < threadpool_->size ; cursor++){
        // create arg
        struct ThreadArg* arg = memory_alloc(sizeof(struct ThreadArg));
        arg->pool = threadpool_;
        arg->index = cursor;

        // start thread
        threadpool_->signal[cursor] = 1;
        if(threadpool_->pool[cursor]->start(threadpool_->pool[cursor], thread_loop, arg) != 0){
            result = -1;
            break;
        }
    }

    return result;
}
int threadpool_post(struct ThreadPool* self, void (*function)(void*), void* arg) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_ *) self;

    // allocate threaditem
    struct ThreadItem* threaditem = memory_alloc(sizeof(struct ThreadItem));
    threaditem->function = function;
    threaditem->arg = arg;

    // enqueue threaditem
    int result = threadpool_->queue->enqueue(threadpool_->queue, threaditem);

    return result;
}
int threadpool_stop(struct ThreadPool* self, int force) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_ *) self;

    // stop all of the threads
    int result = 0;
    for(int cursor = 0 ; cursor < threadpool_->size ; cursor++){
        threadpool_->signal[cursor] = 0;
        if(force){
            if(threadpool_->pool[cursor]->stop(threadpool_->pool[cursor]) != 0){
                result = -1;
                break;
            }
        }
    }

    // wait for exiting
    if(!force){
        threadpool_->semaphore->wait(threadpool_->semaphore, threadpool_->size);
    }

    return result;
}

struct ThreadPool* threadpool_new(int size) {
    struct ThreadPool_* threadpool_ = memory_alloc(sizeof(struct ThreadPool_));

    // init private methods
    threadpool_->self.start = threadpool_start;
    threadpool_->self.post = threadpool_post;
    threadpool_->self.stop = threadpool_stop;

    // init internal queue and threads
    threadpool_->semaphore = semaphore_new(0);
    threadpool_->queue = queue_new(2, -1, NULL);
    threadpool_->pool = memory_alloc(size * sizeof(struct Thread*));
    threadpool_->signal = memory_alloc(size * sizeof(int));
    threadpool_->size = size;
    for(int cursor = 0 ; cursor < size ; cursor++){
        threadpool_->pool[cursor] = thread_new();
    }

    return (struct ThreadPool *) threadpool_;
}
void threadpool_free(struct ThreadPool* threadpool) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_ *) threadpool;

    // destroy internal queue and threads
    for(int cursor = 0 ; cursor < threadpool_->size; cursor++){
        thread_free(threadpool_->pool[cursor]);
    }
    memory_free(threadpool_->pool);
    memory_free(threadpool_->signal);
    queue_free(threadpool_->queue);
    semaphore_free(threadpool_->semaphore);

    memory_free(threadpool_);
}