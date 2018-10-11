#include <low/Mutex.h>

#if defined(APP_WINDOWS)

#include <low/Date.h>
#include <low/Heap.h>
#include <low/String.h>
#include <low/Thread.h>

struct Mutex_ {
    // self public object
    Mutex self;

    // constructor data
    String* name;

    // private data
    void* memory;
    HANDLE memory_handle;
    HANDLE mutex;
    HANDLE semaphore;
};

// vtable
Mutex_VTable* mutex_vtable_normal;
Mutex_VTable* mutex_vtable_recursive;
Mutex_VTable* mutex_vtable_errorcheck;

// link methods
int mutex_acquire_normal(struct Mutex* self, uint_64 timeout);
int mutex_release_normal(struct Mutex* self);

int mutex_acquire_recursive(struct Mutex* self, uint_64 timeout);
int mutex_release_recursive(struct Mutex* self);

int mutex_acquire_errorcheck(struct Mutex* self, uint_64 timeout);
int mutex_release_errorcheck(struct Mutex* self);

// local methods
void* mutex_errorcheck_anonymous_new();
void mutex_errorcheck_anonymous_free(void* memory);
void* mutex_errorcheck_named_new(char* name, HANDLE* memory_handle);
void mutex_errorcheck_named_free(void* memory, HANDLE memory_handle);

// implement methods
void* mutex_errorcheck_anonymous_new() {
    // alocate thread id
    void* result = heap_alloc(sizeof(int));

    int* id = result;

    // init thread id
    *id = -1;

    return result;
}
void mutex_errorcheck_anonymous_free(void* memory) {
    heap_free(memory);
}
void* mutex_errorcheck_named_new(char* name, HANDLE* memory_handle) {
    // check share memory exists
    bool exists = false;
    HANDLE exists_handle = OpenFileMappingA(INVALID_HANDLE_VALUE, FALSE, name);
    if (exists_handle != INVALID_HANDLE_VALUE) {
        // exists, it was created before
        CloseHandle(exists_handle);
        exists = true;
    }

    // allocate share thread id
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

    // get thread id
    int* id = result;

    // create and init thread id or open
    if (!exists) {
        // init share thread id
        *id = -1;
    }

    return result;
}
void mutex_errorcheck_named_free(void* memory, HANDLE memory_handle) {
    // unmap share memory
    UnmapViewOfFile(memory);

    // close share memory
    CloseHandle(memory_handle);
}

// normal mode vtable operators
int mutex_acquire_normal(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // aquire the win32 semaphore
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(mutex_->semaphore, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed - try
        if (WaitForSingleObject(mutex_->semaphore, timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int mutex_release_normal(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // release the win32 semaphore
    if (ReleaseSemaphore(mutex_->semaphore, 1, NULL) != 0) {
        return 0;
    }

    return -1;
}

// recursive mode vtable operators
int mutex_acquire_recursive(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // aquire the win32 mutex
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(mutex_->mutex, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }
    } else {
        // timed - try
        if (WaitForSingleObject(mutex_->mutex, timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int mutex_release_recursive(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // release the win32 mutex
    if (ReleaseMutex(mutex_->mutex) != 0) {
        return 0;
    }

    return -1;
}

// errorcheck mode vtable operators
int mutex_acquire_errorcheck(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // get thread id address
    int* id = mutex_->memory;

    // aquire the win32 mutex
    if (timeout == UINT_64_MAX) {
        // infinity
        if (WaitForSingleObject(mutex_->mutex, INFINITE) == WAIT_OBJECT_0) {
            // error check: saved thread id is not thread id, and then set it
            if (thread_id() != *id) {
                *id = thread_id();
                return 0;
            } else {
                ReleaseMutex(mutex_->mutex);
            }
        }
    } else {
        // timed - try
        if (WaitForSingleObject(mutex_->mutex, timeout) == WAIT_OBJECT_0) {
            // error check: saved thread id is not thread id, and then set it
            if (thread_id() != *id) {
                *id = thread_id();
                return 0;
            } else {
                ReleaseMutex(mutex_->mutex);
            }
        }
    }

    return -1;
}
int mutex_release_errorcheck(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // release the win32 mutex
    if (ReleaseMutex(mutex_->mutex) != 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void mutex_init() {
    // init normal vtable
    mutex_vtable_normal = heap_alloc(sizeof(Mutex_VTable));
    mutex_vtable_normal->acquire = mutex_acquire_normal;
    mutex_vtable_normal->release = mutex_release_normal;

    // init recursive vtable
    mutex_vtable_recursive = heap_alloc(sizeof(Mutex_VTable));
    mutex_vtable_recursive->acquire = mutex_acquire_recursive;
    mutex_vtable_recursive->release = mutex_release_recursive;

    // init errorcheck vtable
    mutex_vtable_errorcheck = heap_alloc(sizeof(Mutex_VTable));
    mutex_vtable_errorcheck->acquire = mutex_acquire_errorcheck;
    mutex_vtable_errorcheck->release = mutex_release_errorcheck;
}
Mutex* mutex_new(int mode) {
    struct Mutex_* mutex_ = heap_alloc(sizeof(struct Mutex_));

    // set vtable
    switch (mode) {
        case 0:
            mutex_->self.vtable = mutex_vtable_normal;
            break;
        case 1:
            mutex_->self.vtable = mutex_vtable_recursive;
            break;
        case 2:
            mutex_->self.vtable = mutex_vtable_errorcheck;
            break;
    }

    // set constructor data
    mutex_->name = NULL;

    // set private data
    mutex_->memory = NULL;
    mutex_->memory_handle = INVALID_HANDLE_VALUE;
    mutex_->mutex = INVALID_HANDLE_VALUE;
    mutex_->semaphore = INVALID_HANDLE_VALUE;

    return (Mutex*)mutex_;
}
void mutex_free(Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    // free private data
    if (mutex_->memory != NULL) {
        if (mutex_->name != NULL) {
            // try acquire critical mutex
            if (critical != NULL) {
                critical->vtable->acquire(critical, UINT_64_MAX);
            }

            // destroy and close or close internal share thread id
            mutex_errorcheck_named_free(mutex_->memory, mutex_->memory_handle);

            // try release critical mutex
            if (critical != NULL) {
                critical->vtable->release(critical);
            }
        } else {
            // destroy internal thread id
            mutex_errorcheck_anonymous_free(mutex_->memory);
        }
    }
    if (mutex_->mutex != INVALID_HANDLE_VALUE) {
        CloseHandle(mutex_->mutex);
    }
    if (mutex_->semaphore != INVALID_HANDLE_VALUE) {
        CloseHandle(mutex_->semaphore);
    }

    // free constructor data
    if (mutex_->name != NULL) {
        string_free(mutex_->name);
    }

    // free self
    heap_free(mutex_);
}
Mutex* mutex_new_object(int mode, char* name) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex_new(mode);

    // set constructor data
    if (name != NULL) {
        mutex_->name = string_new_printf("%s_mutex", name);
    }

    // set private data
    switch (mode) {
        case 0:
            // init normal semaphore
            mutex_->semaphore = CreateSemaphoreA(
                NULL,
                1,
                1,
                mutex_->name->vtable->value(mutex_->name));
            break;
        case 1:
            // init recursive mutex
            mutex_->mutex = CreateMutexA(
                NULL,
                FALSE,
                mutex_->name->vtable->value(mutex_->name));
            break;
        case 2:
            // set thread id
            if (name != NULL) {
                // try acquire critical mutex
                if (critical != NULL) {
                    critical->vtable->acquire(critical, UINT_64_MAX);
                }

                // create and init or open internal share thread id
                mutex_->memory = mutex_errorcheck_named_new(mutex_->name->vtable->value(mutex_->name), &mutex_->memory_handle);

                // try release critical mutex
                if (critical != NULL) {
                    critical->vtable->release(critical);
                }
            } else {
                // create internal thread id
                mutex_->memory = mutex_errorcheck_anonymous_new();
            }

            // init recursive mutex
            mutex_->mutex = CreateMutexA(
                NULL,
                FALSE,
                mutex_->name->vtable->value(mutex_->name));
            break;
    }

    return (Mutex*)mutex_;
}

#endif