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
    tsize item;

    // private data
    Process** pool;
    Message* message;
};

struct ProcessPool_Message {
    void (*function)(void* item);
};

// vtable
ProcessPool_VTable* processpool_vtable;

// link methods
int processpool_start(ProcessPool* self);
int processpool_post(ProcessPool* self, void* item);
int processpool_stop(ProcessPool* self);

// local methods
int processpool_looper(ProcessPool* self);

// implement methods
int processpool_looper(ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // open parent message queue
    String* message_name = string_new_printf("pool_%d", process_get_parent());
    processpool_->message = message_new_named(message_name->vtable->value(message_name), 1024, processpool_->item);
    string_free(message_name);

    // allocate temp for items (function pointer + arg values)
    struct ProcessPool_Message* item = heap_alloc(processpool_->item);

    // start looper
    while (processpool_->message->vtable->dequeue(processpool_->message, item, UINT_64_MAX) == 0) {
        // check message is termination
        if(item->function == NULL){
            break;
        }

        // run function with item
        item->function(item);
    }

    heap_free(item);

    message_free(processpool_->message);

    return 0;
}

// vtable operators
int processpool_start(ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // start process pool
    int result = 0;
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        // start process
        if (processpool_->pool[cursor]->vtable->start(processpool_->pool[cursor], (int (*)(void*))processpool_looper, self) != 0) {
            result = -1;
        }
    }

    return result;
}
int processpool_post(ProcessPool* self, void* item) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // post message to queue
    int result = processpool_->message->vtable->enqueue(processpool_->message, item, UINT_64_MAX);

    return result;
}
int processpool_stop(ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // enqueue null message and wait for all processes to exit
    int result = 0;
    struct ProcessPool_Message message = {NULL};
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        processpool_->message->vtable->enqueue(processpool_->message, &message, UINT_64_MAX);
    }

    // wait for processes to stop
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        if(processpool_->pool[cursor]->vtable->join(processpool_->pool[cursor]) != 0){
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
    processpool_->item = 0;

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
ProcessPool* processpool_new_object(int size, tsize item) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)processpool_new();

    // set constructor data
    processpool_->size = size;
    processpool_->item = item;

    // set private data
    processpool_->pool = heap_alloc(sizeof(Process*) * size);
    for (int cursor = 0; cursor < size; cursor++) {
        processpool_->pool[cursor] = process_new_object();
    }

    // create message queue
    String* message_name = string_new_printf("pool_%d", process_get_self());
    processpool_->message = message_new_named(message_name->vtable->value(message_name), 1024, processpool_->item);
    string_free(message_name);

    return (ProcessPool*)processpool_;
}
