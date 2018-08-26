#ifdef __unix__

#include <low/itc/low/Mutex.h>

#include <io/memory/Memory.h>
#include <low/local/Time.h>
#include <pthread.h>

struct Mutex_ {
    struct Mutex self;
    pthread_mutex_t mutex;
};

// link methods
int mutex_lock(struct Mutex* self);
int mutex_timelock(struct Mutex* self, long int timeout);
int mutex_unlock(struct Mutex* self);

int mutex_lock(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // lock the pthread mutex
    pthread_mutex_lock(&(mutex_->mutex));

    return 0;
}
int mutex_timelock(struct Mutex* self, long int timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // get start time
    long int time = time_epochmillis();

    // try lock until timeout
    while ((time_epochmillis() - time) <= timeout) {
        if (pthread_mutex_trylock(&(mutex_->mutex)) == 0) {
            return 0;
        }
    }

    return -1;
}
int mutex_unlock(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // lock the pthread mutex
    pthread_mutex_unlock(&(mutex_->mutex));

    return 0;
}

struct Mutex* mutex_new() {
    struct Mutex_* mutex_ = memory_alloc(sizeof(struct Mutex_));

    // init private methods
    mutex_->self.lock = mutex_lock;
    mutex_->self.timelock = mutex_timelock;
    mutex_->self.unlock = mutex_unlock;

    // init internal mutex
    pthread_mutex_init(&(mutex_->mutex), NULL);

    return (struct Mutex*)mutex_;
}
void mutex_free(struct Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    // destry internal mutex
    pthread_mutex_destroy(&(mutex_->mutex));

    memory_free(mutex_);
}

#endif