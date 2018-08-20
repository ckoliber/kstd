#include <low/concurrency/Lock.h>
#include <stdatomic.h>

struct Lock_ {
    struct Lock this;
    volatile int lock;
};

int lock_lock(struct Lock* lock);
int lock_unlock(struct Lock* lock);
int lock_trylock(struct Lock* lock, int timeout);

int lock_lock(struct Lock* lock) {
    struct Lock_* lock_ = lock;

    return 0;
}

int lock_unlock(struct Lock* lock) {
    struct Lock_* lock_ = lock;

    return 0;
}

int lock_trylock(struct Lock* lock, int timeout) {
    struct Lock_* lock_ = lock;

    return 0;
    return -1;
}

struct Lock* lock_new() {
    struct Lock_* lock_ = memory_alloc(sizeof(struct Lock_));

    // init private methods
    lock_->this.lock = lock_lock;
    lock_->this.unlock = lock_unlock;
    lock_->this.trylock = lock_trylock;

    // init the core of lock (mutex)
    pthread_mutex_init(&(lock_->mutex), NULL);

    return lock_;
}

void lock_free(struct Lock* lock) {
    struct Lock_* lock_ = lock;

    // destry mutex
    pthread_mutex_destroy(&(lock_->mutex));

    memory_free(lock_);
}