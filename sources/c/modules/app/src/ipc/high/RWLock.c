#include <ipc/high/RWLock.h>

#include <dsa/low/String.h>
#include <ipc/low/Mutex.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>

struct RWLock_ {
    // self public object
    RWLock self;

    // private data
    Mutex* critical_mutex;
    Mutex* write_mutex;
    int readers_count;
};

// vtable
RWLock_VTable* rwlock_vtable;

// link methods
int rwlock_read_lock(struct RWLock* self, uint_64 timeout);
int rwlock_read_unlock(struct RWLock* self);
int rwlock_write_lock(struct RWLock* self, uint_64 timeout);
int rwlock_write_unlock(struct RWLock* self);

// implement methods
// vtable operators
int rwlock_read_lock(struct RWLock* self, uint_64 timeout) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // get time and result
    int result = -1;
    uint_64 time = time_epochmillis();

    // critical begin
    if (rwlock_->critical_mutex->vtable->acquire(rwlock_->critical_mutex, timeout) == 0) {
        // add readers count
        rwlock_->readers_count++;

        // check readers count
        if (rwlock_->readers_count == 1) {
            // get new timeout and lock writer
            timeout -= (time_epochmillis() - time);
            result = rwlock_->write_mutex->vtable->acquire(rwlock_->write_mutex, timeout);
        } else {
            result = 0;
        }

        // critical end
        rwlock_->critical_mutex->vtable->release(rwlock_->critical_mutex);
    }

    return result;
}
int rwlock_read_unlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // critical begin
    rwlock_->critical_mutex->vtable->acquire(rwlock_->critical_mutex, UINT_16_MAX);

    // decrease readers count and release write lock
    int result = -1;
    rwlock_->readers_count--;
    if (rwlock_->readers_count == 0) {
        result = rwlock_->write_mutex->vtable->release(rwlock_->write_mutex);
    } else {
        result = 0;
    }

    // critical end
    rwlock_->critical_mutex->vtable->release(rwlock_->critical_mutex);

    return result;
}
int rwlock_write_lock(struct RWLock* self, uint_64 timeout) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // write lock
    int result = rwlock_->write_mutex->vtable->acquire(rwlock_->write_mutex, timeout);

    return result;
}
int rwlock_write_unlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // write unlock
    int result = rwlock_->write_mutex->vtable->release(rwlock_->write_mutex);

    return result;
}

// object allocation and deallocation operators

void rwlock_init() {
    // init vtable
    rwlock_vtable = heap_alloc(sizeof(RWLock_VTable));
    rwlock_vtable->read_lock = rwlock_read_lock;
    rwlock_vtable->read_unlock = rwlock_read_unlock;
    rwlock_vtable->write_lock = rwlock_write_lock;
    rwlock_vtable->write_unlock = rwlock_write_unlock;
}
RWLock* rwlock_new() {
    struct RWLock_* rwlock_ = heap_alloc(sizeof(struct RWLock_));

    // set vtable
    rwlock_->self.vtable = rwlock_vtable;

    // set constructor data

    // set private data
    rwlock_->critical_mutex = NULL;
    rwlock_->write_mutex = NULL;
    rwlock_->readers_count = 0;

    return (RWLock*)rwlock_;
}
void rwlock_free(RWLock* rwlock) {
    struct RWLock_* rwlock_ = (struct RWLock_*)rwlock;

    // free private data
    if (rwlock_->critical_mutex != NULL) {
        mutex_free(rwlock_->critical_mutex);
    }
    if (rwlock_->write_mutex != NULL) {
        mutex_free(rwlock_->write_mutex);
    }

    // free self
    heap_free(rwlock_);
}
RWLock* rwlock_new_object(char* name) {
    struct RWLock_* rwlock_ = (struct RWLock_*)rwlock_new();

    // set constructor data

    // set private data

    // create internal critical mutex
    String* rwlock_critical_mutex_name = string_new_concat(name, "/rwlock_critical_mutex");
    rwlock_->critical_mutex = mutex_new(0, rwlock_critical_mutex_name->vtable->value(rwlock_critical_mutex_name));
    string_free(rwlock_critical_mutex_name);

    // create internal write mutex
    String* rwlock_write_mutex_name = string_new_concat(name, "/rwlock_write_mutex");
    rwlock_->write_mutex = mutex_new(0, rwlock_write_mutex_name->vtable->value(rwlock_write_mutex_name));
    string_free(rwlock_write_mutex_name);

    return (RWLock*)rwlock_;
}