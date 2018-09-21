#include <ipc/high/Lock.h>

#include <dsa/low/String.h>
#include <ipc/low/Mutex.h>
#include <memory/low/Heap.h>

struct Lock_ {
    // self public object
    Lock self;

    // constructor data

    // private data
    Mutex* mutex;
};

// vtable
Lock_VTable* lock_vtable;

// link methods
int lock_lock(struct Lock* self, uint_64 timeout);
int lock_unlock(struct Lock* self);

// implement methods
// vtable operators
int lock_lock(struct Lock* self, uint_64 timeout) {
    struct Lock_* lock_ = (struct Lock_*)self;

    // acquire internal Mutex
    int result = lock_->mutex->vtable->acquire(lock_->mutex, timeout);

    return result;
}
int lock_unlock(struct Lock* self) {
    struct Lock_* lock_ = (struct Lock_*)self;

    // release internal Mutex
    int result = lock_->mutex->vtable->release(lock_->mutex);

    return result;
}

// object allocation and deallocation operators
void lock_init() {
    // init vtable
    lock_vtable = heap_alloc(sizeof(Lock_VTable));
    lock_vtable->lock = lock_lock;
    lock_vtable->unlock = lock_unlock;
}
Lock* lock_new() {
    struct Lock_* lock_ = heap_alloc(sizeof(struct Lock_));

    // set vtable
    lock_->self.vtable = lock_vtable;

    // set constructor data

    // set private data
    lock_->mutex = NULL;

    return (Lock*)lock_;
}
void lock_free(Lock* lock) {
    struct Lock_* lock_ = (struct Lock_*)lock;

    // free private data
    if (lock_->mutex != NULL) {
        mutex_free(lock_->mutex);
    }

    // free self
    heap_free(lock_);
}
Lock* lock_new_object(int mode, char* name) {
    struct Lock_* lock_ = (struct Lock_*)lock_new();

    // set constructor data

    // set private data
    String* lock_mutex_name = string_new_concat(name, "/lock_mutex");
    lock_->mutex = mutex_new(mode, lock_mutex_name->vtable->value(lock_mutex_name));
    string_free(lock_mutex_name);

    return (Lock*)lock_;
}