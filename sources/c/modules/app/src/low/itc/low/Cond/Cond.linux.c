#ifdef __unix__

#include <low/itc/low/Cond.h>

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

struct Cond_ {
    struct Cond self;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

// link methods
int cond_wait(struct Cond* self);
int cond_timewait(struct Cond* self, int timeout);
int cond_signal(struct Cond* self);
int cond_broadcast(struct Cond* self);

int cond_wait(struct Cond* self) {
    struct Cond_* cond_ = self;

    // wait on the cond
    pthread_mutex_lock(&(cond_->mutex));
    pthread_cond_wait(&(cond_->cond), &(cond_->mutex));
    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}
int cond_timewait(struct Cond* self, int timeout) {
    struct Cond_* cond_ = self;

    // get timeout in timespec
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = timeout * 1.0e6;

    // timedwait on the cond
    pthread_mutex_lock(&(cond_->mutex));
    int result = pthread_cond_timedwait(&(cond_->cond), &(cond_->mutex), &time);
    pthread_mutex_unlock(&(cond_->mutex));

    return result;
}
int cond_signal(struct Cond* self) {
    struct Cond_* cond_ = self;

    // signal on the cond
    pthread_mutex_lock(&(cond_->mutex));
    pthread_cond_signal(&(cond_->cond));
    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}
int cond_broadcast(struct Cond* self) {
    struct Cond_* cond_ = self;

    // broadcast on the cond
    pthread_mutex_lock(&(cond_->mutex));
    pthread_cond_broadcast(&(cond_->cond));
    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}

struct Cond* cond_new() {
    struct Cond_* cond_ = memory_alloc(sizeof(struct Cond_));

    // init private methods
    cond_->self.wait = cond_wait;
    cond_->self.timewait = cond_timewait;
    cond_->self.signal = cond_signal;
    cond_->self.broadcast = cond_broadcast;

    // init internal cond and mutex
    pthread_cond_init(&(cond_->cond), NULL);
    pthread_mutex_init(&(cond_->mutex), NULL);

    return cond_;
}
void cond_free(struct Cond* cond) {
    struct Cond_* cond_ = cond;

    // destry internal cond and mutex
    pthread_cond_destroy(&(cond_->cond));
    pthread_mutex_destroy(&(cond_->mutex));

    memory_free(cond_);
}

#endif