#include <low/Condition.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/Mutex.h>
#include <low/Semaphore.h>
#include <low/String.h>

struct Condition_ {
    // self public object
    Condition self;

    // constructor data
    String* name;

    // private data
    void* memory;
    HANDLE memory_handle;
    Mutex* critical_mutex;
    Semaphore* sleep_semaphore;
};

// vtable
Condition_VTable* condition_vtable;

// link methods
int condition_wait(struct Condition* self, uint_64 timeout);
int condition_signal(struct Condition* self, int count);

// local methods
void* condition_anonymous_new();
void condition_anonymous_free(void* memory);
void* condition_named_new(char* name, HANDLE* memory_handle);
void condition_named_free(void* memory, HANDLE memory_handle);

// implement methods
void* condition_anonymous_new() {
    // alocate sleepers
    void* result = heap_alloc(sizeof(int));

    // get sleepers address
    int* sleepers = result;

    // init sleepers
    *sleepers = 0;

    return result;
}
void condition_anonymous_free(void* memory) {
    heap_free(memory);
}
void* condition_named_new(char* name, HANDLE* memory_handle) {
    // check share memory exists
    bool exists = false;
    HANDLE exists_handle = OpenFileMappingA(INVALID_HANDLE_VALUE, FALSE, name);
    if (exists_handle != INVALID_HANDLE_VALUE) {
        // exists, it was created before
        CloseHandle(exists_handle);
        exists = true;
    }

    // allocate share sleepers
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
        sizeof(int));

    // check error
    if (result == NULL) {
        CloseHandle(handle);
        return NULL;
    } else {
        *memory_handle = handle;
    }

    // get sleepers
    int* sleepers = result;

    // create and init sleepers or open
    if (!exists) {
        // init share sleepers
        *sleepers = 0;
    }

    return result;
}
void condition_named_free(void* memory, HANDLE memory_handle) {
    // unmap share memory
    UnmapViewOfFile(memory);

    // close share memory
    CloseHandle(memory_handle);
}

// vtable operators
int condition_wait(struct Condition* self, uint_64 timeout) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // get sleepers address
    int* sleepers = condition_->memory;

    // aquire the critical mutex
    condition_->critical_mutex->vtable->acquire(condition_->critical_mutex, UINT_64_MAX);

    // critical section
    (*sleepers)++;

    // release the critical mutex
    condition_->critical_mutex->vtable->release(condition_->critical_mutex);

    // goto sleep wait
    if (condition_->sleep_semaphore->vtable->wait(condition_->sleep_semaphore, timeout) == 0) {
        return 0;
    }

    return -1;
}
int condition_signal(struct Condition* self, int count) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // get sleepers address
    int* sleepers = condition_->memory;

    // aquire the critical mutex
    condition_->critical_mutex->vtable->acquire(condition_->critical_mutex, UINT_64_MAX);

    // critical section
    int result;
    if (count > 0) {
        // signal
        for (int cursor = 0; cursor < count; cursor++) {
            if (*sleepers > 0) {
                // signal to wake up
                condition_->sleep_semaphore->vtable->post(condition_->sleep_semaphore);

                // reduce sleepers
                (*sleepers)--;
            } else {
                break;
            }
        }
        result = 0;
    } else {
        // broadcast
        while (*sleepers > 0) {
            // signal to wake up
            condition_->sleep_semaphore->vtable->post(condition_->sleep_semaphore);

            // reduce sleepers
            (*sleepers)--;
        }
        result = 0;
    }

    // release the critical mutex
    condition_->critical_mutex->vtable->release(condition_->critical_mutex);

    return result;
}

// object allocation and deallocation operators
void condition_init() {
    // init vtable
    condition_vtable = heap_alloc(sizeof(Condition_VTable));
    condition_vtable->wait = condition_wait;
    condition_vtable->signal = condition_signal;
}
Condition* condition_new() {
    struct Condition_* condition_ = heap_alloc(sizeof(struct Condition_));

    // set vtable
    condition_->self.vtable = condition_vtable;

    // set constructor data
    condition_->name = NULL;

    // set private data
    condition_->memory = NULL;
    condition_->memory_handle = INVALID_HANDLE_VALUE;
    condition_->critical_mutex = NULL;
    condition_->sleep_semaphore = NULL;

    return (Condition*)condition_;
}
void condition_free(Condition* condition) {
    struct Condition_* condition_ = (struct Condition_*)condition;

    // free private data
    if (condition_->memory != NULL) {
        if (condition_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // destroy and close or close internal share sleepers
            condition_named_free(condition_->memory, condition_->memory_handle);

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal sleepers
            condition_anonymous_free(condition_->memory);
        }
    }
    if (condition_->critical_mutex != NULL) {
        mutex_free(condition_->critical_mutex);
    }
    if (condition_->sleep_semaphore != NULL) {
        semaphore_free(condition_->sleep_semaphore);
    }

    // free constructor data
    if (condition_->name != NULL) {
        string_free(condition_->name);
    }

    // free self
    heap_free(condition_);
}
Condition* condition_new_object(char* name) {
    struct Condition_* condition_ = (struct Condition_*)condition_new();

    // set constructor data
    if (name != NULL) {
        condition_->name = string_new_printf("%s_condition", name);
    }

    // set private data
    if (name != NULL) {
        // create internal critical mutex
        String* condition_critical_name = string_new_printf("%s_condition_critical", name);
        condition_->critical_mutex = mutex_new_object(0, condition_critical_name->vtable->value(condition_critical_name));
        string_free(condition_critical_name);

        // create internal sleep semaphore
        String* condition_sleep_name = string_new_concat("%s_condition_sleep", name);
        condition_->sleep_semaphore = semaphore_new_object(condition_sleep_name->vtable->value(condition_sleep_name), 0);
        string_free(condition_sleep_name);

        // try acquire critical mutex
        if (critical != NULL) {
            critical->vtable->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share sleepers
        condition_->memory = condition_named_new(condition_->name->vtable->value(condition_->name), &condition_->memory_handle);

        // try release critical mutex
        if (critical != NULL) {
            critical->vtable->release(critical);
        }
    } else {
        // create internal critical mutex
        condition_->critical_mutex = mutex_new_object(0, NULL);

        // create internal sleep semaphore
        condition_->sleep_semaphore = semaphore_new_object(NULL, 0);

        // create internal sleepers
        condition_->memory = condition_anonymous_new();
    }

    return (Condition*)condition_;
}

#endif