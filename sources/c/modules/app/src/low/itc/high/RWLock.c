#include <low/itc/high/RWLock.h>

#include <low/itc/low/Mutex.h>
#include <low/local/Time.h>

struct RWLock_ {
    struct RWLock self;
    struct Mutex* writer_mutex;
    struct Mutex* critical_mutex;
    int readers_count
};

// link methods
int rwlock_readlock(struct RWLock* self);
int rwlock_writelock(struct RWLock* self);
int rwlock_timereadlock(struct RWLock* self, int timeout);
int rwlock_timewritelock(struct RWLock* self, int timeout);
int rwlock_readunlock(struct RWLock* self);
int rwlock_writeunlock(struct RWLock* self);

int rwlock_readlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // reader lock (first critical mutex and if first reader then lock writer mutex)
    rwlock_->critical_mutex->lock(rwlock_->critical_mutex);
    rwlock_->readers_count++;
    if (rwlock_->readers_count == 1) {
        rwlock_->writer_mutex->lock(rwlock_->writer_mutex);
    }
    rwlock_->critical_mutex->unlock(rwlock_->critical_mutex);

    return 0;
}
int rwlock_writelock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // lock the writer mutex
    rwlock_->writer_mutex->lock(rwlock_->writer_mutex);

    return 0;
}
int rwlock_timereadlock(struct RWLock* self, int timeout) {
    struct RWLock_* rwlock_ = self;

    // set default result and current time
    int result = -1;
    long int time = time_epochmillis();

    // reader timelock -> try timelock critical mutex
    if (rwlock_->critical_mutex->timelock(rwlock_->critical_mutex, timeout) == 0) {
        rwlock_->readers_count++;

        // if first reader then try lock writer else complete
        if (rwlock_->readers_count == 1) {
            // get new timeout
            long int timeout_new = timeout - (time_epochmillis() - time);

            // try lock write mutex in new timeout
            if (rwlock_->writer_mutex->timelock(rwlock_->writer_mutex, timeout_new) == 0) {
                result = 0;
            }

        } else {
            result = 0;
        }
        rwlock_->critical_mutex->unlock(rwlock_->critical_mutex);
    }

    return result;
}
int rwlock_timewritelock(struct RWLock* self, int timeout) {
    struct RWLock_* rwlock_ = self;

    // timelock the writer mutex
    int result = rwlock_->writer_mutex->timelock(rwlock_->writer_mutex, timeout);

    return result;
}
int rwlock_readunlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // reader unlock (first critical mutex and if last reader then unlock writer mutex)
    rwlock_->critical_mutex->lock(rwlock_->critical_mutex);
    rwlock_->readers_count--;
    if (rwlock_->readers_count == 0) {
        rwlock_->writer_mutex->unlock(rwlock_->writer_mutex);
    }
    rwlock_->critical_mutex->unlock(rwlock_->critical_mutex);

    return 0;
}
int rwlock_writeunlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // unlock the writer lock
    rwlock_->writer_mutex->unlock(rwlock_->writer_mutex);

    return 0;
}

struct RWLock* rwlock_new() {
    struct RWLock_* rwlock_ = memory_alloc(sizeof(struct RWLock_));

    // init private methods
    rwlock_->self.readlock = rwlock_readlock;
    rwlock_->self.writelock = rwlock_writelock;
    rwlock_->self.timereadlock = rwlock_timereadlock;
    rwlock_->self.timewritelock = rwlock_timewritelock;
    rwlock_->self.readunlock = rwlock_readunlock;
    rwlock_->self.writeunlock = rwlock_writeunlock;

    // create internal Mutexes
    rwlock_->writer_mutex = mutex_new();
    rwlock_->critical_mutex = mutex_new();
    rwlock_->readers_count = 0;

    return rwlock_;
}

void rwlock_free(struct RWLock* rwlock) {
    struct RWLock_* rwlock_ = rwlock;

    // destroy internal Mutexes
    mutex_free(rwlock_->writer_mutex);
    mutex_free(rwlock_->critical_mutex);

    memory_free(rwlock_);
}