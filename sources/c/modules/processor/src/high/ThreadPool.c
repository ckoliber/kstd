#include <high/ThreadPool.h>

#include <low/Heap.h>
#include <low/Message.h>
#include <low/Thread.h>
#include <stdio.h>

struct ThreadPool_ {
    // self public object
    ThreadPool self;

    // constructor data
    int size;
    tsize arg;

    // private data
    Thread** pool;
    Message* message;
};

struct ThreadPool_Message{
    void (*function)(void* arg);
};

// vtable
ThreadPool_VTable* threadpool_vtable;

// link methods
int threadpool_start(ThreadPool* self);
int threadpool_post(ThreadPool* self, void (*function)(void*), void* arg);
int threadpool_stop(ThreadPool* self);

// local methods
int threadpool_looper(ThreadPool* self);

// implement methods
int threadpool_looper(ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // allocate temp for items (function pointer + arg value)
    struct ThreadPool_Message* message = heap_alloc(sizeof(struct ThreadPool_Message) + threadpool_->arg);

    // start looper
    while (threadpool_->message->vtable->dequeue(threadpool_->message, message, UINT_64_MAX) == 0) {
        // get arg address
        void* arg = message + sizeof(struct ThreadPool_Message);

        printf("GET = %p\n", message->function);
        fflush(stdout);

        // run function with arg
        message->function(arg);
    }

    heap_free(message);

    return 0;
}

// vtable operators
int threadpool_start(ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // start thread pool
    int result = 0;
    for (int cursor = 0; cursor < threadpool_->size; cursor++) {
        // start thread
        if (threadpool_->pool[cursor]->vtable->start(threadpool_->pool[cursor], (int (*)(void *)) threadpool_looper, self) != 0) {
            result = -1;
        }
    }

    return result;
}
int threadpool_post(ThreadPool* self, void (*function)(void*), void* arg) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    printf("POST = %p\n", function);
    fflush(stdout);

    // allocate temp for item package
    struct ThreadPool_Message* message = heap_alloc(sizeof(struct ThreadPool_Message) + threadpool_->arg);

    // fill temp package item
    message->function = function;
    heap_copy(message + sizeof(struct ThreadPool_Message), arg, threadpool_->arg);

    // post message to queue
    int result = threadpool_->message->vtable->enqueue(threadpool_->message, message, UINT_64_MAX);

    // free temp package item
    heap_free(message);

    return result;
}
int threadpool_stop(ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // stop thread pool
    int result = 0;
    for (int cursor = 0; cursor < threadpool_->size; cursor++) {
        // stop thread
        if (threadpool_->pool[cursor]->vtable->stop(threadpool_->pool[cursor]) != 0) {
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
    threadpool_->arg = 0;

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
ThreadPool* threadpool_new_object(int size, tsize arg) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)threadpool_new();

    // set constructor data
    threadpool_->size = size;
    threadpool_->arg = arg;

    // set private data
    threadpool_->pool = heap_alloc(sizeof(ThreadPool*) * size);
    for (int cursor = 0; cursor < size; cursor++) {
        threadpool_->pool[cursor] = thread_new_object(0);
    }

    // create message queue
    threadpool_->message = message_new_object(NULL, 1024, sizeof(struct ThreadPool_Message) + arg);

    return (ThreadPool*)threadpool_;
}