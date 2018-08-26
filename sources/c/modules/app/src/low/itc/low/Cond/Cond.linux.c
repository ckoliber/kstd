#ifdef __unix__

#include <low/itc/low/Cond.h>

#include <io/memory/Memory.h>
#include <low/local/Time.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

struct Cond_ {
    struct Cond self;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
};

// link methods
int cond_wait(struct Cond* self, int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2);
int cond_timewait(struct Cond* self, int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2, long int timeout);
int cond_signal(struct Cond* self, void (*critical)(void*), void* arg);
int cond_broadcast(struct Cond* self, void (*critical)(void*), void* arg);

int cond_wait(struct Cond* self, int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2) {
    struct Cond_* cond_ = (struct Cond_*)self;

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
    if (critical != NULL) {
        critical(arg2);
    }

    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}
int cond_timewait(struct Cond* self, int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2, long int timeout) {
    struct Cond_* cond_ = (struct Cond_*)self;

    // timedwait on the cond
    pthread_mutex_lock(&(cond_->mutex));

    // get timeout in timespec and result
    int result = 0;
    struct timeval time_now;
    struct timespec time_out;
    gettimeofday(&time_now, NULL);
    time_out.tv_sec = time_now.tv_sec;
    time_out.tv_nsec = time_now.tv_usec * 1000;
    time_out.tv_sec += timeout / 1000;
    time_out.tv_nsec += (timeout % 1000) * 1000000;

    // run conditional wait loop
    while (condition == NULL || condition(arg1)) {
        if (pthread_cond_timedwait(&(cond_->cond), &(cond_->mutex), &time_out) != 0) {
            result = -1;
            break;
        }
        if (condition == NULL) {
            break;
        }
    }

    // run critical
    if (result == 0) {
        if (critical != NULL) {
            critical(arg2);
        }
    }

    pthread_mutex_unlock(&(cond_->mutex));

    return result;
}
int cond_signal(struct Cond* self, void (*critical)(void*), void* arg) {
    struct Cond_* cond_ = (struct Cond_*)self;

    pthread_mutex_lock(&(cond_->mutex));

    // signal on the cond
    pthread_cond_signal(&(cond_->cond));

    // run critical
    if (critical != NULL) {
        critical(arg);
    }

    pthread_mutex_unlock(&(cond_->mutex));

    return 0;
}
int cond_broadcast(struct Cond* self, void (*critical)(void*), void* arg) {
    struct Cond_* cond_ = (struct Cond_*)self;

    pthread_mutex_lock(&(cond_->mutex));

    // signal on the cond
    pthread_cond_broadcast(&(cond_->cond));

    // run critical
    if (critical != NULL) {
        critical(arg);
    }

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

    return (struct Cond*)cond_;
}
void cond_free(struct Cond* cond) {
    struct Cond_* cond_ = (struct Cond_*)cond;

    // destry internal cond and mutex
    pthread_cond_destroy(&(cond_->cond));
    pthread_mutex_destroy(&(cond_->mutex));

    memory_free(cond_);
}

#endif