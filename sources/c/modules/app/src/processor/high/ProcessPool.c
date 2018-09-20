#include <processor/high/ProcessPool.h>

#include <dsa/low/String.h>
#include <ipc/low/Message.h>
#include <memory/low/Heap.h>
#include <processor/low/Process.h>

struct ProcessPool_ {
    ProcessPool self;
    int size;
    tsize arg;
    Process** pool;
    Message* message;
};

struct ProcessArg {
    tsize arg;
};

// link methods
int processpool_start(struct ProcessPool* self);
int processpool_post(struct ProcessPool* self, void (*function)(void*), void* arg);
int processpool_stop(struct ProcessPool* self);

// local methods
int processpool_looper(struct ProcessArg* process_arg);

// implement methods
int processpool_looper(struct ProcessArg* process_arg) {
    // allocate temp for items (function pointer + arg value)
    void* item = heap_alloc(sizeof(void (*)(void*)) + process_arg->arg);

    // open message queue
    String* message_name = string_new_printf("/pool-%d", process_self());
    Message* message = message_new(message_name->value(message_name), 1024, process_arg->arg);
    string_free(message_name);

    // start looper
    int result = 0;
    while (message->dequeueu(message, item, UINT_64_MAX) == 0) {
        // extract function and arg from package
        void (*function)(void*) = item;
        void* arg = item + sizeof(void (*)(void*));

        // run function with arg
        function(arg);
    }

    message_free(message);
    heap_free(process_arg);

    return result;
}

int processpool_start(struct ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // start process pool
    int result = 0;
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        // init process arg
        struct ProcessArg* process_arg = heap_alloc(sizeof(struct ProcessArg));
        process_arg->arg = processpool_->arg;

        // start process
        if (processpool_->pool[cursor]->start(processpool_->pool[cursor], processpool_looper, process_arg) != 0) {
            result = -1;
        }
    }

    return result;
}
int processpool_post(struct ProcessPool* self, void (*function)(void*), void* arg) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // allocate temp for item package
    void* item = heap_alloc(sizeof(void (*)(void*)) + processpool_->arg);

    // fill temp package item
    heap_copy(item, function, sizeof(void (*)(void*)));
    heap_copy(item + sizeof(void (*)(void*)), arg, processpool_->arg);

    // post message to queue
    int result = processpool_->message->enqueue(processpool_->message, item, UINT_64_MAX);

    // free temp package item
    heap_free(item);

    return result;
}
int processpool_stop(struct ProcessPool* self) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)self;

    // stop process pool
    int result = 0;
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        // stop process
        if (processpool_->pool[cursor]->stop(processpool_->pool[cursor]) != 0) {
            result = -1;
        }
    }

    return result;
}

ProcessPool* processpool_new(int size, tsize arg) {
    struct ProcessPool_* processpool_ = heap_alloc(sizeof(struct ProcessPool_));

    // init private methods
    processpool_->self.start = processpool_start;
    processpool_->self.post = processpool_post;
    processpool_->self.stop = processpool_stop;

    // create internal processes
    processpool_->size = size;
    processpool_->arg = arg;
    processpool_->pool = heap_alloc(sizeof(ProcessPool*) * size);
    for (int cursor = 0; cursor < size; cursor++) {
        processpool_->pool[cursor] = process_new();
    }

    // create message queue
    String* message_name = string_new_printf("/pool-%d", process_self());
    processpool_->message = message_new(message_name->value(message_name), 1024, arg);
    string_free(message_name);

    return (ProcessPool*)processpool_;
}
void processpool_free(ProcessPool* processpool) {
    struct ProcessPool_* processpool_ = (struct ProcessPool_*)processpool;

    // destroy internal processes
    for (int cursor = 0; cursor < processpool_->size; cursor++) {
        process_free(processpool_->pool[cursor]);
    }
    heap_free(processpool_->pool);

    // destroy message queue
    message_free(processpool_->message);

    heap_free(processpool_);
}