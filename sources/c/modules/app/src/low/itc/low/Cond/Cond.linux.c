#ifdef __unix__

#include <low/itc/low/Cond.h>

#include <low/local/Time.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

struct Cond_ {
    struct Cond self;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

// link methods
int cond_wait(struct Cond* self, int (*condition)(void* arg1), void* arg1, void (*critical)(void* arg2), void* arg2);
int cond_timewait(struct Cond* self, int (*condition)(void* arg1), void* arg1, void (*critical)(void* arg2), void* arg2, int timeout);
int cond_signal(struct Cond* self, void (*critical)(void* arg), void* arg);
int cond_broadcast(struct Cond* self, void (*critical)(void* arg), void* arg);

int cond_wait(struct Cond* self, int (*condition)(void* arg1), void* arg1, void (*critical)(void* arg2), void* arg2) {
    struct Cond_* cond_ = self;

    // wait on the cond
    pthread_mutex_lock(&(cond_->mutex));

    // run conditional wait loop
    while (condition == NULL || condition(arg1)) {
        pthread_cond_wait(&(cond_->cond), &(cond_->mutex));
        if (condition == NULL) {
            break;
        }
    }

    // run critical
    critical(arg2);

    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}
int cond_timewait(struct Cond* self, int (*condition)(void* arg1), void* arg1, void (*critical)(void* arg2), void* arg2, int timeout) {
    struct Cond_* cond_ = self;

    // timedwait on the cond
    pthread_mutex_lock(&(cond_->mutex));

    // get timeout in timespec
    struct timespec time_out;
    time_out.tv_sec = 0;
    time_out.tv_nsec = 0;

    // get current time
    int result = 0;
    long int time = time_epochmillis();

    // run conditional wait loop
    while (condition == NULL || condition(arg1)) {
        // set new timeout and timewait
        time_out.tv_nsec = (timeout - (time_epochmillis() - time)) * 1.0e6;
        if (pthread_cond_timedwait(&(cond_->cond), &(cond_->mutex), &time_out) != 0) {
            result = -1;
            break;
        }
        if (condition == NULL) {
            break;
        }
    }

    // run critical
    critical(arg2);

    pthread_mutex_unlock(&(cond_->mutex));

    return result;
}
int cond_signal(struct Cond* self, void (*critical)(void* arg), void* arg) {
    struct Cond_* cond_ = self;

    pthread_mutex_lock(&(cond_->mutex));

    // signal on the cond
    pthread_cond_signal(&(cond_->cond));

    // run critical
    critical(arg);

    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}
int cond_broadcast(struct Cond* self, void (*critical)(void* arg), void* arg) {
    struct Cond_* cond_ = self;

    pthread_mutex_lock(&(cond_->mutex));

    // signal on the cond
    pthread_cond_broadcast(&(cond_->cond));

    // run critical
    critical(arg);

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