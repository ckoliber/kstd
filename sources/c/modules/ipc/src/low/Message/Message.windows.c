#include <low/Message.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/Semaphore.h>
#include <low/String.h>

struct Message_ {
    // self public object
    Message self;

    // constructor data
    String* name;
    int max;
    tsize item;

    // private data
    void* memory;
    HANDLE memory_handle;
    Semaphore* full_semaphore;
    Semaphore* empty_semaphore;
};

// vtable
Message_VTable* message_vtable;

// link methods
int message_enqueue(Message* self, void* item, uint_64 timeout);
int message_dequeue(Message* self, void* item, uint_64 timeout);

// local methods
void* message_anonymous_new(int max, tsize item);
void message_anonymous_free(void* memory);
void* message_named_new(char* name, HANDLE* memory_handle, int max, tsize item);
void message_named_free(void* memory, HANDLE memory_handle);

// implement methods
void* message_anonymous_new(int max, tsize item) {
    // alocate start and end and queue
    void* result = heap_alloc(sizeof(int) + sizeof(int) + (item * max));

    // get start and end and queue address
    int* start = result;
    int* end = result + sizeof(int);
    void* queue = result + sizeof(int) + sizeof(int);

    // init start and end
    *start = 0;
    *end = 0;

    return result;
}
void message_anonymous_free(void* memory) {
    heap_free(memory);
}
void* message_named_new(char* name, HANDLE* memory_handle, int max, tsize item) {
    // check share memory exists
    bool exists = false;
    HANDLE exists_handle = OpenFileMappingA(INVALID_HANDLE_VALUE, FALSE, name);
    if (exists_handle != INVALID_HANDLE_VALUE) {
        // exists, it was created before
        CloseHandle(exists_handle);
        exists = true;
    }

    // allocate share start and end and queue
    HANDLE handle = CreateFileMappingA(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_EXECUTE_READWRITE,
        0,
        0,
        name);
    void* result = MapViewOfFile(
        handle,
        FILE_MAP_ALL_ACCESS,
        0,
        0,
        sizeof(int) + sizeof(int) + (item * max));

    // check error
    if (result == NULL) {
        CloseHandle(handle);
        return NULL;
    } else {
        *memory_handle = handle;
    }

    // get start and end and queue
    int* start = result;
    int* end = result + sizeof(int);
    void* queue = result + sizeof(int) + sizeof(int);

    // create and init start and end or open
    if (!exists) {
        // init share start and end
        *start = 0;
        *end = 0;
    }

    return result;
}
void message_named_free(void* memory, HANDLE memory_handle) {
    // unmap share memory
    UnmapViewOfFile(memory);

    // close share memory
    CloseHandle(memory_handle);
}

// vtable operators
int message_enqueue(Message* self, void* item, uint_64 timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get start and end and queue address
    int* start = message_->memory;
    int* end = message_->memory + sizeof(int);
    void* queue = message_->memory + sizeof(int) + sizeof(int);

    // wait on full semaphore
    if (message_->full_semaphore->vtable->wait(message_->full_semaphore, timeout) == 0) {
        // add item to queue
        heap_copy(queue + *end, item, message_->item);
        *end = (*end + 1) % message_->max;

        // signal on empty semaphore
        return message_->empty_semaphore->vtable->post(message_->empty_semaphore);
    }

    return -1;
}
int message_dequeue(Message* self, void* item, uint_64 timeout) {
    struct Message_* message_ = (struct Message_*)self;

    // get start and end and queue address
    int* start = message_->memory;
    int* end = message_->memory + sizeof(int);
    void* queue = message_->memory + sizeof(int) + sizeof(int);

    // wait on empty semaphore
    if (message_->empty_semaphore->vtable->wait(message_->empty_semaphore, timeout) == 0) {
        // add item to queue
        heap_copy(item, queue + *start, message_->item);
        *start = (*start + 1) % message_->max;

        // signal on full semaphore
        return message_->full_semaphore->vtable->post(message_->full_semaphore);
    }

    return -1;
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
    message_->memory_handle = INVALID_HANDLE_VALUE;
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
            message_named_free(message_->memory, message_->memory_handle);

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
    struct Message_* message_ = (struct Message_*)message_new();

    // set constructor data
    if (name != NULL) {
        message_->name = string_new_printf("%s_message", name);
    }
    message_->max = max;
    message_->item = item;

    // set private data
    if (name != NULL) {
        // create internal full semaphore
        String* message_full_name = string_new_printf("%s_message_full", name);
        message_->full_semaphore = semaphore_new_object(message_full_name->vtable->value(message_full_name), max);
        string_free(message_full_name);

        // create internal empty semaphore
        String* message_empty_name = string_new_concat("%s_message_empty", name);
        message_->empty_semaphore = semaphore_new_object(message_empty_name->vtable->value(message_empty_name), 0);
        string_free(message_empty_name);

        // try acquire critical mutex
        if (critical != NULL) {
            critical->vtable->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share message queue
        message_->memory = message_named_new(message_->name->vtable->value(message_->name), &message_->memory_handle, max, item);

        // try release critical mutex
        if (critical != NULL) {
            critical->vtable->release(critical);
        }
    } else {
        // create internal full semaphore
        message_->full_semaphore = semaphore_new_object(NULL, max);

        // create internal empty semaphore
        message_->empty_semaphore = semaphore_new_object(NULL, 0);

        // create internal message queue
        message_->memory = message_anonymous_new(max, item);
    }

    return (Message*)message_;
}

#endif