#include <low/concurrency/RWLock.h>

#include <low/concurrency/Lock.h>

struct RWLock_ {
    struct RWLock this;
    struct Lock* writer_lock;
    struct Lock* critical_lock;
    int readers_count
};

// link methods
int rwlock_rlock(struct RWLock* self);
int rwlock_wlock(struct RWLock* self);
int rwlock_runlock(struct RWLock* self);
int rwlock_wunlock(struct RWLock* self);

int rwlock_rlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // reader lock (first critical lock and if first reader then lock writer)
    rwlock_->critical_lock->lock(rwlock_->critical_lock);
    rwlock_->readers_count++;
    if (rwlock_->readers_count == 1) {
        rwlock_->writer_lock->lock(rwlock_->writer_lock);
    }
    rwlock_->critical_lock->unlock(rwlock_->critical_lock);

    return 0;
}

int rwlock_wlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // lock the writer lock
    rwlock_->writer_lock->lock(rwlock_->writer_lock);

    return 0;
}

int rwlock_runlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // reader unlock (first critical lock and if last reader then unlock writer)
    rwlock_->critical_lock->lock(rwlock_->critical_lock);
    rwlock_->readers_count--;
    if (rwlock_->readers_count == 0) {
        rwlock_->writer_lock->unlock(rwlock_->writer_lock);
    }
    rwlock_->critical_lock->unlock(rwlock_->critical_lock);

    return 0;
}

int rwlock_wunlock(struct RWLock* self) {
    struct RWLock_* rwlock_ = self;

    // unlock the writer lock
    rwlock_->writer_lock->unlock(rwlock_->writer_lock);

    return 0;
}

struct RWLock* rwlock_new() {
    struct RWLock_* rwlock_ = memory_alloc(sizeof(struct RWLock_));

    // init private methods
    rwlock_->this.rlock = rwlock_rlock;
    rwlock_->this.wlock = rwlock_wlock;
    rwlock_->this.runlock = rwlock_runlock;
    rwlock_->this.wunlock = rwlock_wunlock;

    // init internal locks and counter
    rwlock_->writer_lock = lock_new();
    rwlock_->critical_lock = lock_new();
    rwlock_->readers_count = 0;

    return rwlock_;
}

void rwlock_free(struct RWLock* rwlock) {
    struct RWLock_* rwlock_ = rwlock;

    // free allocated locks
    lock_free(rwlock_->writer_lock);
    lock_free(rwlock_->critical_lock);

    memory_free(rwlock_);
}