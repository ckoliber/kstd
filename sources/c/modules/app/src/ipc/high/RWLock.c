#include <low/itc/high/RWLock.h>

#include <io/memory/Memory.h>
#include <low/itc/low/Mutex.h>
#include <low/local/Time.h>

struct RWLock_ {
    struct RWLock self;
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
            result = rwlock_->writer_mutex->acquire(rwlock_->writer_mutex, timeout);
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
        result = rwlock_->writer_mutex->release(rwlock_->writer_mutex);
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
    int result = rwlock_->writer_mutex->acquire(rwlock_->writer_mutex, timeout);

    return result;
}
int rwlock_write_unlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = (struct RWLock_*)self;

    // write unlock
    int result = rwlock_->writer_mutex->release(rwlock_->writer_mutex);

    return result;
}

RWLock* rwlock_new(char* name) {
    struct RWLock_* rwlock_ = memory_alloc(sizeof(struct RWLock_));

    // init private methods
    rwlock_->self.readlock = rwlock_readlock;
    rwlock_->self.writelock = rwlock_writelock;
    rwlock_->self.timereadlock = rwlock_timereadlock;
    rwlock_->self.timewritelock = rwlock_timewritelock;
    rwlock_->self.readunlock = rwlock_readunlock;
    rwlock_->self.writeunlock = rwlock_writeunlock;

    // create internal mutexes
    String* criticalmutex_name = string_new_concat(name, "/criticalmutex");
    rwlock_->critical_mutex = mutex_new(criticalmutex_name);
    string_free(criticalmutex_name);

    // create internal mutexes
    String* writemutex_name = string_new_concat(name, "/writemutex");
    rwlock_->writer_mutex = mutex_new(writemutex_name);
    string_free(writemutex_name);

    rwlock_->readers_count = 0;

    return (struct RWLock*)rwlock_;
}

void rwlock_free(RWLock* rwlock) {
    struct RWLock_* rwlock_ = (struct RWLock_*)rwlock;

    // destroy internal mutexes
    mutex_free(rwlock_->critical_mutex);
    mutex_free(rwlock_->write_mutex);

    heap_free(rwlock_);
}