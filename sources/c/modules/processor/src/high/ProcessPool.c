#include <high/ProcessPool.h>

#include <low/Heap.h>
#include <low/Message.h>
#include <low/Process.h>
#include <low/String.h>

struct ProcessPool_ {
    // self public object
    ProcessPool self;

    // constructor data
    int size;
    tsize arg;

    // private data
    Process** pool;
    Message* message;
};

// vtable
ProcessPool_VTable* processpool_vtable;

// link methods
int processpool_start(ProcessPool* self);
int processpool_post(ProcessPool* self, void (*function)(void*), void* arg);
int processpool_stop(ProcessPool* self);

// local methods
int processpool_looper(ProcessPool* self);

// implement methods
int processpool_looper(ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // open parent message queue
    String* message_name = string_new_printf("pool_%d", process_parent());
    Message* message = message_new_object(message_name->vtable->value(message_name), 1024, processpool_->arg);
    string_free(message_name);

    // allocate temp for items (function pointer + arg value)
    void* item = heap_alloc(sizeof(void (*)(void*)) + processpool_->arg);

    // start looper
    while (message->vtable->dequeueu(message, item, UINT_64_MAX) == 0) {
        // get function and arg address
        void (*function)(void*) = item;
        void* arg = item + sizeof(void (*)(void*));

        // run function with arg
        function(arg);
    }

    heap_free(item);
    message_free(message);

    return 0;
}

// vtable operators
int processpool_start(ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // start process pool
    int result = 0;
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        // start process
        if (processpool_->pool[cursor]->vtable->start(processpool_->pool[cursor], (int (*)(void *)) processpool_looper, self) != 0) {
            result = -1;
        }
    }

    return result;
}
int processpool_post(ProcessPool* self, void (*function)(void*), void* arg) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // allocate temp for item package
    void* item = heap_alloc(sizeof(void (*)(void*)) + processpool_->arg);

    // fill temp package item
    heap_copy(item, function, sizeof(void (*)(void*)));
    heap_copy(item + sizeof(void (*)(void*)), arg, processpool_->arg);

    // post message to queue
    int result = processpool_->message->vtable->enqueue(processpool_->message, item, UINT_64_MAX);

    // free temp package item
    heap_free(item);

    return result;
}
int processpool_stop(ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // stop process pool
    int result = 0;
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        // stop process
        if (processpool_->pool[cursor]->vtable->stop(processpool_->pool[cursor]) != 0) {
            result = -1;
        }
    }

    return result;
}

// object allocation and deallocation operators
void processpool_init() {
    // init vtable
    processpool_vtable = heap_alloc(sizeof(ProcessPool_VTable));
    processpool_vtable->start = processpool_start;
    processpool_vtable->post = processpool_post;
    processpool_vtable->stop = processpool_stop;
}
ProcessPool* processpool_new() {
    struct ProcessPool_* processpool_ = heap_alloc(sizeof(struct ProcessPool_));

    // set vtable
    processpool_->self.vtable = processpool_vtable;

    // set constructor data
    processpool_->size = 0;
    processpool_->arg = 0;

    // set private data
    processpool_->pool = NULL;
    processpool_->message = NULL;

    return (ProcessPool*)processpool_;
}
void processpool_free(ProcessPool* processpool) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)processpool;

    // free private data

    // destroy internal processes
    if (processpool_->pool != NULL) {
        for (int cursor = 0; cursor < processpool_->size; cursor++) {
            process_free(processpool_->pool[cursor]);
        }
        heap_free(processpool_->pool);
    }
    if (processpool_->message != NULL) {
        message_free(processpool_->message);
    }

    // free self
    heap_free(processpool_);
}
ProcessPool* processpool_new_object(int size, tsize arg) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)processpool_new();

    // set constructor data
    processpool_->size = size;
    processpool_->arg = arg;

    // set private data
    processpool_->pool = heap_alloc(sizeof(ProcessPool*) * size);
    for (int cursor = 0; cursor < size; cursor++) {
        processpool_->pool[cursor] = process_new();
    }

    // create message queue
    String* message_name = string_new_printf("pool_%d", process_self());
    processpool_->message = message_new_object(message_name->vtable->value(message_name), 1024, arg);
    string_free(message_name);

    return (ProcessPool*)processpool_;
}