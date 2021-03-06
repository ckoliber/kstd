#include <high/ThreadPool.h>

#include <low/Heap.h>
#include <low/Message.h>
#include <low/Thread.h>

struct ThreadPool_ {
    // self public object
    ThreadPool self;

    // constructor data
    int size;
    tsize item;

    // private data
    Thread** pool;
    Message* message;
};

struct ThreadPool_Message {
    void (*function)(void* item);
};

// vtable
ThreadPool_VTable* threadpool_vtable;

// link methods
int threadpool_start(ThreadPool* self);
int threadpool_post(ThreadPool* self, void* item);
int threadpool_stop(ThreadPool* self);

// local methods
int threadpool_looper(ThreadPool* self);

// implement methods
int threadpool_looper(ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // allocate temp for items (function pointer + arg value)
    struct ThreadPool_Message* item = heap_alloc(threadpool_->item);

    // start looper
    while (threadpool_->message->vtable->dequeue(threadpool_->message, item, UINT_64_MAX) == 0) {
        // check message is termination
        if(item->function == NULL){
            break;
        }

        // run function with arg
        item->function(item);
    }

    heap_free(item);

    return 0;
}

// vtable operators
int threadpool_start(ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // start thread pool
    int result = 0;
    for (int cursor = 0; cursor < threadpool_->size; cursor++) {
        // start thread
        if (threadpool_->pool[cursor]->vtable->start(threadpool_->pool[cursor], (int (*)(void*))threadpool_looper, self) != 0) {
            result = -1;
        }
    }

    return result;
}
int threadpool_post(ThreadPool* self, void* item) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // post message to queue
    int result = threadpool_->message->vtable->enqueue(threadpool_->message, item, UINT_64_MAX);

    return result;
}
int threadpool_stop(ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // enqueue null message and wait for all threads to exit
    int result = 0;
    struct ThreadPool_Message message = {NULL};
    for (int cursor = 0; cursor < threadpool_->size; cursor++) {
        threadpool_->message->vtable->enqueue(threadpool_->message, &message, UINT_64_MAX);
    }

    // wait for threads to stop
    for (int cursor = 0; cursor < threadpool_->size; cursor++) {
        if(threadpool_->pool[cursor]->vtable->join(threadpool_->pool[cursor]) != 0){
            result = -1;
        }
    }

    return result;
}

// object allocation and deallocation operators
void threadpool_init() {
    // init vtable
    threadpool_vtable = heap_alloc(sizeof(ThreadPool_VTable));
    threadpool_vtable->start = threadpool_start;
    threadpool_vtable->post = threadpool_post;
    threadpool_vtable->stop = threadpool_stop;
}
ThreadPool* threadpool_new() {
    struct ThreadPool_* threadpool_ = heap_alloc(sizeof(struct ThreadPool_));

    // set vtable
    threadpool_->self.vtable = threadpool_vtable;

    // set constructor data
    threadpool_->size = 0;
    threadpool_->item = 0;

    // set private data
    threadpool_->pool = NULL;
    threadpool_->message = NULL;

    return (ThreadPool*)threadpool_;
}
void threadpool_free(ThreadPool* threadpool) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)threadpool;

    // free private data

    // destroy internal threads
    if (threadpool_->pool != NULL) {
        for (int cursor = 0; cursor < threadpool_->size; cursor++) {
            thread_free(threadpool_->pool[cursor]);
        }
        heap_free(threadpool_->pool);
    }
    if (threadpool_->message != NULL) {
        message_free(threadpool_->message);
    }

    // free self
    heap_free(threadpool_);
}
ThreadPool* threadpool_new_object(int size, tsize item) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)threadpool_new();

    // set constructor data
    threadpool_->size = size;
    threadpool_->item = item;

    // set private data
    threadpool_->pool = heap_alloc(sizeof(Thread*) * size);
    for (int cursor = 0; cursor < size; cursor++) {
        threadpool_->pool[cursor] = thread_new_object(0);
    }

    // create message queue
    threadpool_->message = message_new_anonymous(1024, threadpool_->item);

    return (ThreadPool*)threadpool_;
}
