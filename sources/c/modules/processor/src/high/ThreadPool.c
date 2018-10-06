#include <processor/high/ThreadPool.h>

#include <ipc/low/Message.h>
#include <memory/low/Heap.h>
#include <processor/low/Thread.h>

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

struct ThreadArg {
    Message* message;
    tsize arg;
};

// vtable
ThreadPool_VTable* threadpool_vtable;

// link methods
int threadpool_start(struct ThreadPool* self);
int threadpool_post(struct ThreadPool* self, void (*function)(void*), void* arg);
int threadpool_stop(struct ThreadPool* self);

// local methods
int threadpool_looper(struct ThreadArg* thread_arg);

// implement methods
int threadpool_looper(struct ThreadArg* thread_arg) {
    // allocate temp for items (function pointer + arg value)
    void* item = heap_alloc(sizeof(void (*)(void*)) + thread_arg->arg);

    // start looper
    int result = 0;
    while (thread_arg->message->vtable->dequeueu(thread_arg->message, item, UINT_64_MAX) == 0) {
        // extract function and arg from package
        void (*function)(void*) = item;
        void* arg = item + sizeof(void (*)(void*));

        // run function with arg
        function(arg);
    }

    heap_free(thread_arg);

    return result;
}

// vtable operators
int threadpool_start(struct ThreadPool* self) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // start thread pool
    int result = 0;
    for (int cursor = 0; cursor < threadpool_->size; cursor++) {
        // init thread arg
        struct ThreadArg* thread_arg = heap_alloc(sizeof(struct ThreadArg));
        thread_arg->message = threadpool_->message;
        thread_arg->arg = threadpool_->arg;

        // start thread
        if (threadpool_->pool[cursor]->vtable->start(threadpool_->pool[cursor], threadpool_looper, thread_arg) != 0) {
            result = -1;
        }
    }

    return result;
}
int threadpool_post(struct ThreadPool* self, void (*function)(void*), void* arg) {
    struct ThreadPool_* threadpool_ = (struct ThreadPool_*)self;

    // allocate temp for item package
    void* item = heap_alloc(sizeof(void (*)(void*)) + threadpool_->arg);

    // fill temp package item
    heap_copy(item, function, sizeof(void (*)(void*)));
    heap_copy(item + sizeof(void (*)(void*)), arg, threadpool_->arg);

    // post message to queue
    int result = threadpool_->message->vtable->enqueue(threadpool_->message, item, UINT_64_MAX);

    // free temp package item
    heap_free(item);

    return result;
}
int threadpool_stop(struct ThreadPool* self) {
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
        threadpool_->pool[cursor] = thread_new(0);
    }

    // create message queue
    threadpool_->message = message_new(NULL, 1024, arg);

    return (ThreadPool*)threadpool_;
}