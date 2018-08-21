#ifdef __unix__

#include <low/itc/low/Mutex.h>

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>

struct Mutex_ {
    struct Mutex self;
    pthread_mutex_t mutex;
};

// link methods
int mutex_lock(struct Mutex* self);
int mutex_timelock(struct Mutex* self, int timeout);
int mutex_unlock(struct Mutex* self);

// local methods
uint64_t micro_time();

uint64_t micro_time() {
    struct timeval time = {0, 0};
    gettimeofday(&time, NULL);
    return time.tv_sec * (uint64_t)1.0e6 + time.tv_usec;
}

int mutex_lock(struct Mutex* self) {
    struct Mutex_* mutex_ = self;

    // lock the pthread mutex
    pthread_mutex_lock(&(mutex_->mutex));

    return 0;
}
int mutex_timelock(struct Mutex* self, int timeout) {
    struct Mutex_* mutex_ = self;

    // get start time

    uint64_t time = micro_time();

    // try lock until timeout
    while (micro_time() - time <= timeout * 1.0e3) {
        if (pthread_mutex_trylock(&(mutex_->mutex)) == 0) {
            return 0;
        }
    }

    return -1;
}
int mutex_unlock(struct Mutex* self) {
    struct Mutex_* mutex_ = self;

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

    return mutex_;
}
void mutex_free(struct Mutex* mutex) {
    struct Mutex_* mutex_ = mutex;

    // destry internal mutex
    pthread_mutex_destroy(&(mutex_->mutex));

    memory_free(mutex_);
}

#endif