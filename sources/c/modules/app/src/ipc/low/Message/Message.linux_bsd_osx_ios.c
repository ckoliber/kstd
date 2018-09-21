#include <ipc/low/Message.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <dsa/low/String.h>
#include <fcntl.h>
#include <ipc/low/Mutex.h>
#include <ipc/low/Semaphore.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <sys/mman.h>

struct Message_ {
    // self public object
    Message self;

    // constructor data
    String* name;
    int max;
    tsize item;

    // private data
    void* memory;
    Semaphore* full_semaphore;
    Semaphore* empty_semaphore;
};

// vtable
Message_VTable* message_vtable;

// link methods
int message_enqueue(struct Message* self, void* item, uint64_t timeout);
int message_dequeue(struct Message* self, void* item, uint64_t timeout);

// local methods
void* message_anonymous_new(int max, tsize item);
void message_anonymous_free(void* memory);
void* message_named_new(char* name, int max, tsize item);
void message_named_free(void* memory, char* name, int max, tsize item);

// implement methods
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
    // check share memory exists
    bool exists = true;
    int exists_fd = shm_open(name, O_CREAT | O_EXCL, 0660);
    if (exists_fd > 0) {
        // not exists, create it
        close(exists_fd);
        exists = false;
    }

    // alocate share start and end and queue and connections
    int fd = shm_open(name, O_CREAT | O_RDWR, 0660);
    void* result = mmap(NULL, sizeof(int) + sizeof(int) + (item * max) + sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    close(fd);

    // check error
    if (result == NULL || result == MAP_FAILED) {
        return NULL;
    }

    // get start and end and queue and connections address
    int* start = result;
    int* end = result + sizeof(int);
    int* queue = result + sizeof(int) + sizeof(int);
    int* connections = result + sizeof(int) + sizeof(int) + (item * max);

    // create and init start and end or open and increase connections
    if (!exists) {
        *connections = 1;

        // init share start and end
        *start = 0;
        *end = 0;
    } else {
        *connections += 1;
    }

    return result;
}
void message_named_free(void* memory, char* name, int max, tsize item) {
    // get connections address
    int* connections = memory + sizeof(int) + sizeof(int) + (item * max);

    // destroy share memory on close all connections
    if (connections <= 1) {
        munmap(memory, sizeof(int) + sizeof(int) + (item * max) + sizeof(int));
        shm_unlink(name);
    } else {
        *connections -= 1;
        munmap(memory, sizeof(int) + sizeof(int) + (item * max) + sizeof(int));
    }
}

// vtable operators
int message_enqueue(struct Message* self, void* item, uint64_t timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get start and end and queue address
    int* start = message_->memory;
    int* end = message_->memory + sizeof(int);
    void* queue = message_->memory + sizeof(int) + sizeof(int);

    // wait on full semaphore
    int result = -1;
    if (message_->full_semaphore->vtable->wait(message_->full_semaphore, timeout) == 0) {
        // add item to queue
        heap_copy(queue + *end, item, message_->item);
        *end = (*end + 1) % message_->max;

        // signal on empty semaphore
        result = message_->empty_semaphore->vtable->post(message_->empty_semaphore);
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
    if (message_->empty_semaphore->vtable->wait(message_->empty_semaphore, timeout) == 0) {
        // add item to queue
        heap_copy(item, queue + *start, message_->item);
        *start = (*start + 1) % message_->max;

        // signal on full semaphore
        result = message_->full_semaphore->vtable->post(message_->full_semaphore);
    }

    return result;
}

// object allocation and deallocation operators
void message_init() {
    // init vtable
    message_vtable = heap_alloc(sizeof(Message_VTable));
    message_vtable->enqueue = message_enqueue;
    message_vtable->dequeueu = message_dequeue;
}
Message* message_new() {
    struct Message_* message_ = heap_alloc(sizeof(struct Message_));

    // set vtable
    message_->self.vtable = message_vtable;

    // set constructor data
    message_->name = NULL;
    message_->max = 0;
    message_->item = 0;

    // set private data
    message_->memory = NULL;
    message_->full_semaphore = NULL;
    message_->empty_semaphore = NULL;

    return (Message*)message_;
}
void message_free(Message* message) {
    struct Message_* message_ = (struct Message_*)message;

    // free private data
    if (message_->memory != NULL) {
        if (message_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // destroy and close or close internal share message queue
            message_named_free(message_->memory, message_->name->vtable->value(message_->name), message_->max, message_->item);

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal message queue
            message_anonymous_free(message_->memory);
        }
    }
    if (message_->full_semaphore != NULL) {
        semaphore_free(message_->full_semaphore);
    }
    if (message_->empty_semaphore != NULL) {
        semaphore_free(message_->empty_semaphore);
    }

    // free constructor data
    if (message_->name != NULL) {
        string_free(message_->name);
    }

    // free self
    heap_free(message_);
}
Message* message_new_object(char* name, int max, tsize item) {
    struct Message_* message_ = (struct Condition_*)message_new();

    // set constructor data
    if (name != NULL) {
        message_->name = string_new_concat(name, "/message");
    }
    message_->max = max;
    message_->item = item;

    // set private data
    if (name != NULL) {
        // create internal full semaphore
        String* full_semaphore_name = string_new_concat(name, "/message/full_semaphore");
        message_->full_semaphore = semaphore_new(full_semaphore_name->vtable->value(full_semaphore_name));
        message_->full_semaphore->vtable->init(message_->full_semaphore, max);
        string_free(full_semaphore_name);

        // create internal empty semaphore
        String* empty_semaphore_name = string_new_concat(name, "/message/empty_semaphore");
        message_->empty_semaphore = semaphore_new(empty_semaphore_name->vtable->value(empty_semaphore_name));
        message_->empty_semaphore->vtable->init(message_->empty_semaphore, 0);
        string_free(empty_semaphore_name);

        // try acquire critical mutex
        if (critical != NULL) {
            critical->vtable->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share message queue
        message_->memory = message_named_new(message_->name->vtable->value(message_->name), max, item);

        // try release critical mutex
        if (critical != NULL) {
            critical->vtable->release(critical);
        }
    } else {
        // create internal full semaphore
        message_->full_semaphore = semaphore_new(NULL);
        message_->full_semaphore->vtable->init(message_->full_semaphore, max);

        // create internal empty semaphore
        message_->empty_semaphore = semaphore_new(NULL);
        message_->empty_semaphore->vtable->init(message_->empty_semaphore, 0);

        // create internal message queue
        message_->memory = message_anonymous_new(max, item);
    }

    return (Message*)message_;
}

#endif