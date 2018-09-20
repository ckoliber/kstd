#include <ipc/low/Message.h>

#if defined(APP_ANDROID)

#include <dsa/low/String.h>
#include <fcntl.h>
#include <ipc/low/Mutex.h>
#include <ipc/low/Semaphore.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <sys/mman.h>

struct Message_ {
    Message self;
    void* memory;
    int max;
    tsize item;
    Semaphore* full_semaphore;
    Semaphore* empty_semaphore;
    String* name;
};

// link methods
int message_enqueue(struct Message* self, void* item, uint64_t timeout);
int message_dequeue(struct Message* self, void* item, uint64_t timeout);

// local methods
void* message_anonymous_new(int max, tsize item);
void message_anonymous_free(void* memory);
void* message_named_new(char* name, int max, tsize item);
void message_named_free(void* memory, char* name, int max, tsize item);

void* message_anonymous_new(int max, tsize item) {
    // alocate start and end and queue
    void* result = heap_alloc(sizeof(int) + sizeof(int) + (item * max));

    // get start and end and queue address
    int* start = result;
    int* end = result + sizeof(int);
    int* queue = result + sizeof(int) + sizeof(int);

    // init start and end
    *start = 0;
    *end = 0;

    return result;
}
void message_anonymous_free(void* memory) {
    heap_free(memory);
}
void* message_named_new(char* name, int max, tsize item) {
    return NULL;
}
void message_named_free(void* memory, char* name, int max, tsize item) {}

// implement methods
int message_enqueue(struct Message* self, void* item, uint64_t timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get start and end and queue address
    int* start = message_->memory;
    int* end = message_->memory + sizeof(int);
    void* queue = message_->memory + sizeof(int) + sizeof(int);

    // wait on full semaphore
    int result = -1;
    if (message_->full_semaphore->wait(message_->full_semaphore, timeout) == 0) {
        // add item to queue
        heap_copy(queue + *end, item, message_->item);
        *end = (*end + 1) % message_->max;

        // signal on empty semaphore
        result = message_->empty_semaphore->post(message_->empty_semaphore);
    }

    return result;
}
int message_dequeue(struct Message* self, void* item, uint64_t timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get start and end and queue address
    int* start = message_->memory;
    int* end = message_->memory + sizeof(int);
    void* queue = message_->memory + sizeof(int) + sizeof(int);

    // wait on empty semaphore
    int result = -1;
    if (message_->empty_semaphore->wait(message_->empty_semaphore, timeout) == 0) {
        // add item to queue
        heap_copy(item, queue + *start, message_->item);
        *start = (*start + 1) % message_->max;

        // signal on full semaphore
        result = message_->full_semaphore->post(message_->full_semaphore);
    }

    return result;
}

Message* message_new(char* name, int max, tsize item) {
    struct Message_* message_ = heap_alloc(sizeof(struct Message_));

    // init private methods
    message_->self.enqueue = message_enqueue;
    message_->self.dequeueu = message_dequeue;

    if (name == NULL) {
        message_->name = NULL;
        message_->max = max;
        message_->item = item;

        // create internal full semaphore
        message_->full_semaphore = semaphore_new(NULL);
        message_->full_semaphore->init(message_->full_semaphore, max);

        // create internal empty semaphore
        message_->empty_semaphore = semaphore_new(NULL);
        message_->empty_semaphore->init(message_->empty_semaphore, 0);

        // create internal message queue
        message_->memory = message_anonymous_new(max, item);
    } else {
        heap_free(message_);
        message_ = NULL;
    }

    return (Message*)message_;
}
void message_free(Message* message) {
    struct Message_* message_ = (struct Message_*)message;

    if (message_->name == NULL) {
        // destroy internal message queue
        message_anonymous_free(message_->memory);
    }

    // destroy internal full and empty semaphore
    semaphore_free(message_->full_semaphore);
    semaphore_free(message_->empty_semaphore);

    heap_free(message_);
}

#endif