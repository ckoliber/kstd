#include <ipc/high/RWLock.h>

#include <dsa/low/String.h>
#include <ipc/low/Mutex.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>

struct RWLock_ {
    RWLock self;
    Mutex* critical_mutex;
    Mutex* write_mutex;
    int readers_count;
};

// link methods
int rwlock_read_lock(struct RWLock* self, uint_64 timeout);
int rwlock_read_unlock(struct RWLock* self);
int rwlock_write_lock(struct RWLock* self, uint_64 timeout);
int rwlock_write_unlock(struct RWLock* self);

// implement methods
int rwlock_read_lock(struct RWLock* self, uint_64 timeout) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // get time and result
    int result = -1;
    uint_64 time = time_epochmillis();

    // critical begin
    if (rwlock_->critical_mutex->acquire(rwlock_->critical_mutex, timeout) == 0) {
        // add readers count
        rwlock_->readers_count++;

        // check readers count
        if (rwlock_->readers_count == 1) {
            // get new timeout and lock writer
            timeout -= (time_epochmillis() - time);
            result = rwlock_->write_mutex->acquire(rwlock_->write_mutex, timeout);
        } else {
            result = 0;
        }

        // critical end
        rwlock_->critical_mutex->release(rwlock_->critical_mutex);
    }

    return result;
}
int rwlock_read_unlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // critical begin
    rwlock_->critical_mutex->acquire(rwlock_->critical_mutex, UINT_16_MAX);

    // decrease readers count and release write lock
    int result = -1;
    rwlock_->readers_count--;
    if (rwlock_->readers_count == 0) {
        result = rwlock_->write_mutex->release(rwlock_->write_mutex);
    } else {
        result = 0;
    }

    // critical end
    rwlock_->critical_mutex->release(rwlock_->critical_mutex);

    return result;
}
int rwlock_write_lock(struct RWLock* self, uint_64 timeout) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // write lock
    int result = rwlock_->write_mutex->acquire(rwlock_->write_mutex, timeout);

    return result;
}
int rwlock_write_unlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // write unlock
    int result = rwlock_->write_mutex->release(rwlock_->write_mutex);

    return result;
}

RWLock* rwlock_new(char* name) {
    struct RWLock_* rwlock_ = heap_alloc(sizeof(struct RWLock_));

    // init private methods
    rwlock_->self.read_lock = rwlock_read_lock;
    rwlock_->self.read_unlock = rwlock_read_unlock;
    rwlock_->self.write_lock = rwlock_write_lock;
    rwlock_->self.write_unlock = rwlock_write_unlock;

    // create internal critical mutex
    String* rwlock_critical_mutex_name = string_new_concat(name, "/rwlock_critical_mutex");
    rwlock_->critical_mutex = mutex_new(0, rwlock_critical_mutex_name->value(rwlock_critical_mutex_name));
    string_free(rwlock_critical_mutex_name);

    // create internal write mutex
    String* rwlock_write_mutex_name = string_new_concat(name, "/rwlock_write_mutex");
    rwlock_->write_mutex = mutex_new(0, rwlock_write_mutex_name->value(rwlock_write_mutex_name));
    string_free(rwlock_write_mutex_name);

    rwlock_->readers_count = 0;

    return (RWLock*)rwlock_;
}
void rwlock_free(RWLock* rwlock) {
    struct RWLock_* rwlock_ = (struct RWLock_*)rwlock;

    // destroy internal mutexes
    mutex_free(rwlock_->critical_mutex);
    mutex_free(rwlock_->write_mutex);

    heap_free(rwlock_);
}