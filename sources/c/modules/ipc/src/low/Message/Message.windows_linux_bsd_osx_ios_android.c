#include <low/Message.h>

#if defined(APP_WINDOWS) || defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Heap.h>
#include <low/MutexLock.h>
#include <low/ReentrantLock.h>
#include <low/Semaphore.h>
#include <low/Share.h>
#include <low/String.h>

struct Message_ {
    // self public object
    Message self;

    // constructor data
    int max;
    tsize item;

    // private data
    Share* share;
    Semaphore* full;
    Semaphore* empty;
    MutexLock* critical;
};

struct Message_Memory {
    int begin;
    int end;
};

// vtable
Message_VTable* message_vtable;

// link methods

int message_enqueue(Message* self, void* item, uint_64 timeout);
int message_dequeue(Message* self, void* item, uint_64 timeout);
int message_size(Message* self, uint_64 timeout);

// implement methods
// vtable operators
int message_enqueue(Message* self, void* item, uint_64 timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get memory and queue address
    struct Message_Memory* memory = message_->share->vtable->address(message_->share);
    char* queue = (char*) memory + sizeof(struct Message_Memory);

    // wait on full semaphore
    if (message_->full->vtable->wait(message_->full, timeout) == 0) {
        // begin critical
        message_->critical->vtable->lock(message_->critical, timeout);

        // add item to queue
        heap_copy(queue + (memory->end * message_->item), item, message_->item);
        memory->end = (memory->end + 1) % message_->max;

        // end critical
        message_->critical->vtable->unlock(message_->critical);

        // signal on empty semaphore
        return message_->empty->vtable->post(message_->empty);
    }

    return -1;
}
int message_dequeue(Message* self, void* item, uint_64 timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get memory and queue address
    struct Message_Memory* memory = message_->share->vtable->address(message_->share);
    char* queue = (char*) memory + sizeof(struct Message_Memory);

    // wait on empty semaphore
    if (message_->empty->vtable->wait(message_->empty, timeout) == 0) {
        // begin critical
        message_->critical->vtable->lock(message_->critical, timeout);

        // remove item from queue
        heap_copy(item, queue + (memory->begin * message_->item), message_->item);
        memory->begin = (memory->begin + 1) % message_->max;

        // end critical
        message_->critical->vtable->unlock(message_->critical);

        // signal on full semaphore
        return message_->full->vtable->post(message_->full);
    }

    return -1;
}
int message_size(Message* self, uint_64 timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get memory and queue address
    struct Message_Memory* memory = message_->share->vtable->address(message_->share);

    // begin critical
    message_->critical->vtable->lock(message_->critical, timeout);

    // get message size
    int result = memory->end - memory->begin;

    // end critical
    message_->critical->vtable->unlock(message_->critical);

    return result;
}

// object allocation and deallocation operators
void message_init() {
    // init vtable
    message_vtable = heap_alloc(sizeof(Message_VTable));
    message_vtable->enqueue = message_enqueue;
    message_vtable->dequeue = message_dequeue;
    message_vtable->size = message_size;
}
Message* message_new() {
    struct Message_* message_ = heap_alloc(sizeof(struct Message_));

    // set vtable
    message_->self.vtable = message_vtable;

    // set constructor data
    message_->max = 0;
    message_->item = 0;

    // set private data
    message_->share = NULL;
    message_->full = NULL;
    message_->empty = NULL;
    message_->critical = NULL;

    return (Message*)message_;
}
void message_free(Message* message) {
    struct Message_* message_ = (struct Message_*)message;

    // free private data
    if (message_->share != NULL) {
        share_free(message_->share);
    }

    // free constructor data

    // free self
    heap_free(message_);
}
Message* message_new_anonymous(int max, tsize item){
    struct Message_* message_ = (struct Message_*)message_new();

    // set constructor data
    message_->max = max;
    message_->item = item;

    // set private data
    // open full semaphore
    message_->full = semaphore_new_anonymous(max);

    // open empty semaphore
    message_->empty = semaphore_new_anonymous(0);

    // open critical mutexlock
    message_->critical = mutexlock_new_anonymous();

    // open errorcheck lock
    message_->share = share_new_anonymous(sizeof(struct Message_Memory) + (message_->max * message_->max), 0);

    // if share connections is 1, init share
    if (message_->share->vtable->connections(message_->share) <= 1) {
        // get memory address
        struct Message_Memory* memory = message_->share->vtable->address(message_->share);

        // init begin
        memory->begin = 0;

        // init end
        memory->end = 0;
    }

    return (Message*)message_;
}
Message* message_new_named(char* name, int max, tsize item){
    struct Message_* message_ = (struct Message_*)message_new();

    // set constructor data
    message_->max = max;
    message_->item = item;

    // set private data
    // open share full semaphore
    String* message_full_name = string_new_printf("%s_ms_f", name);
    message_->full = semaphore_new_named(message_full_name->vtable->value(message_full_name), max);
    string_free(message_full_name);

    // open share empty semaphore
    String* message_empty_name = string_new_printf("%s_ms_e", name);
    message_->empty = semaphore_new_named(message_empty_name->vtable->value(message_empty_name), 0);
    string_free(message_empty_name);

    // open share critical mutexlock
    String* message_critical_name = string_new_printf("%s_ms_c", name);
    message_->critical = mutexlock_new_named(message_critical_name->vtable->value(message_critical_name));
    string_free(message_critical_name);

    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // open share errorcheck lock
    String* message_name = string_new_printf("%s_ms", name);
    message_->share = share_new_named(message_name->vtable->value(message_name), sizeof(struct Message_Memory) + (message_->max * message_->max), 0);
    string_free(message_name);

    // if share connections is 1, init share
    if (message_->share->vtable->connections(message_->share) <= 1) {
        // get memory address
        struct Message_Memory* memory = message_->share->vtable->address(message_->share);

        // init begin
        memory->begin = 0;

        // init end
        memory->end = 0;
    }

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Message*)message_;
}

#endif
