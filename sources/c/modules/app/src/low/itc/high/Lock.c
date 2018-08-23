#include <low/itc/high/Lock.h>

#include <io/memory/Memory.h>
#include <low/itc/low/Mutex.h>

struct Lock_ {
    struct Lock self;
    struct Mutex* mutex;
};

// link methods
int lock_lock(struct Lock* self);
int lock_timelock(struct Lock* self, int timeout);
int lock_unlock(struct Lock* self);

int lock_lock(struct Lock* self) {
    struct Lock_* lock_ = self;

    // lock internal Mutex
    lock_->mutex->lock(lock_->mutex);

    return 0;
}
int lock_timelock(struct Lock* self, int timeout) {
    struct Lock_* lock_ = self;

    // lock internal Mutex
    int result = lock_->mutex->timelock(lock_->mutex, timeout);

    return result;
}
int lock_unlock(struct Lock* self) {
    struct Lock_* lock_ = self;

    // lock internal Mutex
    lock_->mutex->unlock(lock_->mutex);

    return 0;
}

struct Lock* lock_new() {
    struct Lock_* lock_ = memory_alloc(sizeof(struct Lock_));

    // init private methods
    lock_->self.lock = lock_lock;
    lock_->self.timelock = lock_timelock;
    lock_->self.unlock = lock_unlock;

    // create internal Mutex
    lock_->mutex = mutex_new();

    return lock_;
}
void lock_free(struct Lock* lock) {
    struct Lock_* lock_ = lock;

    // destry internal Mutex
    mutex_free(lock_->mutex);

    memory_free(lock_);
}