#include <ipc/low/Condition.h>

#if defined(APP_LINUX)

#include <dsa/low/String.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <memory/low/Share.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

struct Condition_ {
    struct Condition self;
    pthread_mutex_t anonymous_mutex;
    pthread_cond_t anonymous_cond;
    Share* named_mutex;
    Share* named_cond;
};

// link methods
int condition_wait_anonymous(struct Condition* self, uint_64 timeout);
int condition_signal_anonymous(struct Condition* self, uint_32 count);
int condition_wait_named(struct Condition* self, uint_64 timeout);
int condition_signal_named(struct Condition* self, uint_32 count);

// implement methods
int condition_wait_anonymous(struct Condition* self, uint_64 timeout) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(condition_->anonymous_mutex));

    // wait the pthread cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_cond_wait(&(condition_->anonymous_cond), &(condition_->anonymous_mutex)) == 0) {
            result = 0;
        }
    } else {
        // timed
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;
        if (pthread_cond_timedwait(&(condition_->anonymous_cond), &(condition_->anonymous_mutex), &time_out) == 0) {
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(&(condition_->anonymous_mutex));

    return result;
}
int condition_signal_anonymous(struct Condition* self, uint_32 count) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(condition_->anonymous_mutex));

    // signal the pthread cond
    int result = -1;
    if (count == UINT_32_MAX) {
        // broadcast
        if (pthread_cond_broadcast(&(condition_->anonymous_cond)) == 0) {
            result = 0;
        }
    } else {
        // signal
        for (int cursor = 0; cursor < (int)count; cursor++) {
            pthread_cond_signal(&(condition_->anonymous_cond));
        }
        result = 0;
    }

    // release the pthread mutex
    pthread_mutex_unlock(&(condition_->anonymous_mutex));

    return result;
}
int condition_wait_named(struct Condition* self, uint_64 timeout) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // aquire the pthread shared mutex
    pthread_mutex_lock(condition_->named_mutex->address(condition_->named_mutex));

    // wait the pthread shared cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_cond_wait(condition_->named_cond->address(condition_->named_cond), condition_->named_mutex->address(condition_->named_mutex)) == 0) {
            result = 0;
        }
    } else {
        // timed
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;
        if (pthread_cond_timedwait(condition_->named_cond->address(condition_->named_cond), condition_->named_mutex->address(condition_->named_mutex), &time_out) == 0) {
            result = 0;
        }
    }

    // release the pthread shared mutex
    pthread_mutex_unlock(condition_->named_mutex->address(condition_->named_mutex));

    return result;
}
int condition_signal_named(struct Condition* self, uint_32 count) {
    struct Condition_* condition_ = (struct Condition_*)self;

    // aquire the pthread named mutex
    pthread_mutex_lock(condition_->named_mutex->address(condition_->named_mutex));

    // signal the pthread shared cond
    int result = -1;
    if (count == UINT_32_MAX) {
        // broadcast
        if (pthread_cond_broadcast(condition_->named_cond->address(condition_->named_cond)) == 0) {
            result = 0;
        }
    } else {
        // signal
        for (int cursor = 0; cursor < (int)count; cursor++) {
            pthread_cond_signal(condition_->named_cond->address(condition_->named_cond));
        }
        result = 0;
    }

    // release the pthread named mutex
    pthread_mutex_unlock(condition_->named_mutex->address(condition_->named_mutex));

    return result;
}

Condition* condition_new(char* name) {
    struct Condition_* condition_ = heap_alloc(sizeof(struct Condition_));

    if (name == NULL) {
        // init private methods
        condition_->self.wait = condition_wait_anonymous;
        condition_->self.signal = condition_signal_anonymous;

        // init internal mutex
        condition_->named_mutex = NULL;
        pthread_mutex_init(&(condition_->anonymous_mutex), NULL);

        // init internal cond
        condition_->named_cond = NULL;
        pthread_cond_init(&(condition_->anonymous_cond), NULL);
    } else {
        // init private methods
        condition_->self.wait = condition_wait_named;
        condition_->self.signal = condition_signal_named;

        // init internal share mutex
        String* mutex_name = string_new_concat(name, "/mutex");
        condition_->named_mutex = share_new(mutex_name->value(mutex_name), sizeof(pthread_mutex_t));
        string_free(mutex_name);

        // init internal share cond
        String* cond_name = string_new_concat(name, "/cond");
        condition_->named_cond = share_new(cond_name->value(cond_name), sizeof(pthread_cond_t));
        string_free(cond_name);

        // if share mutex not created before, init it
        if (condition_->named_mutex->connections(condition_->named_mutex) <= 1) {
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, 1);
            pthread_mutex_init(condition_->named_mutex->address(condition_->named_mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }

        // if share cond not created before, init it
        if (condition_->named_cond->connections(condition_->named_cond) <= 1) {
            pthread_condattr_t cattr;
            pthread_condattr_init(&cattr);
            pthread_condattr_setpshared(&cattr, 1);
            pthread_cond_init(condition_->named_cond->address(condition_->named_cond), &cattr);
            pthread_condattr_destroy(&cattr);
        }
    }

    return (Condition*)condition_;
}
void condition_free(Condition* condition) {
    struct Condition_* condition_ = (struct Condition_*)condition;

    if (condition_->named_mutex == NULL) {
        // destroy internal mutex, cond
        pthread_mutex_destroy(&(condition_->anonymous_mutex));
        pthread_cond_destroy(&(condition_->anonymous_cond));
    } else {
        // destroy internal share mutex
        if (condition_->named_mutex->connections(condition_->named_mutex) <= 1) {
            pthread_mutex_destroy(condition_->named_mutex->address(condition_->named_mutex));
        }
        share_free(condition_->named_mutex);

        // destroy internal share cond
        if (condition_->named_cond->connections(condition_->named_cond) <= 1) {
            pthread_cond_destroy(condition_->named_cond->address(condition_->named_cond));
        }
        share_free(condition_->named_cond);
    }

    heap_free(condition_);
}

#endif