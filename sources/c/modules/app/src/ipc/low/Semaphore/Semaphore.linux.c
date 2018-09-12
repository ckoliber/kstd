#include <ipc/low/Semaphore.h>

#include <ipc/low/Condition.h>
#include <memory/low/Heap.h>

struct Semaphore_ {
    struct Semaphore self;
    Condition* condition;
    int value;
};

struct Arg {
    struct Semaphore_* semaphore_;
    int count;
    int type;
};

// link methods
int semaphore_wait(struct Semaphore* self, int count);
int semaphore_timewait(struct Semaphore* self, int count, long int timeout);
int semaphore_post(struct Semaphore* self, int count);
int semaphore_get(struct Semaphore* self);

// local methods
int condition(void* arg);
void critical(void* arg);

int condition(void* arg) {
    struct Arg* arg_ = arg;
    return arg_->semaphore_->value < arg_->count;
}
void critical(void* arg) {
    struct Arg* arg_ = arg;
    if (arg_->type == 0) {
        arg_->semaphore_->value -= arg_->count;
    } else {
        arg_->semaphore_->value += arg_->count;
    }
}

int semaphore_wait(struct Semaphore* self, int count) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // wait on cond to signal and decrease value
    struct Arg arg = {semaphore_, count, 0};
    semaphore_->cond->wait(semaphore_->cond, condition, &arg, critical, &arg);

    return 0;
}
int semaphore_timewait(struct Semaphore* self, int count, long int timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // timewait on cond to signal and decrease value
    struct Arg arg = {semaphore_, count, 0};
    int result = semaphore_->cond->timewait(semaphore_->cond, condition, &arg, critical, &arg, timeout);

    return result;
}
int semaphore_post(struct Semaphore* self, int count) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // signal on cond and increase value
    struct Arg arg = {semaphore_, count, 1};
    semaphore_->cond->signal(semaphore_->cond, critical, &arg);

    return 0;
}
int semaphore_get(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get semaphore value
    int result = semaphore_->value;

    return result;
}

struct Semaphore* semaphore_new(int value) {
    struct Semaphore_* semaphore_ = heap_alloc(sizeof(struct Semaphore_));

    // init private methods
    semaphore_->self.wait = semaphore_wait;
    semaphore_->self.timewait = semaphore_timewait;
    semaphore_->self.post = semaphore_post;
    semaphore_->self.get = semaphore_get;

    // init internal Cond and Default Value
    semaphore_->cond = cond_new();
    semaphore_->value = value;

    return (struct Semaphore*)semaphore_;
}

void semaphore_free(struct Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    // destry internal Cond
    cond_free(semaphore_->cond);

    heap_free(semaphore_);
}

#include <ipc/low/Semaphore.h>

#if defined(APP_LINUX)

#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <memory/low/Share.h>
#include <pthread.h>

struct Semaphore_ {
    struct Mutex self;
    pthread_mutex_t anonymous_mutex;
    pthread_cond_t anonymous_cond;
    int anonymous_value;
    Share* named_mutex;
    Share* named_cond;
    Share* named_value;
};

// link methods
int semaphore_wait_anonymous(struct Semaphore* self, uint_64 timeout);
int semaphore_post_anonymous(struct Semaphore* self);
int semaphore_get_anonymous(struct Semaphore* self);
int semaphore_wait_named(struct Semaphore* self, uint_64 timeout);
int semaphore_post_named(struct Semaphore* self);
int semaphore_get_named(struct Semaphore* self);

// implement methods
int semaphore_wait_anonymous(struct Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(semaphore_->anonymous_mutex));

    // wait the pthread cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        while (condition_->anonymous_value == 0) {
            pthread_cond_wait(&(condition_->anonymous_cond), &(condition_->anonymous_mutex));
        }
        condition_->anonymous_value--;
        result = 0;
    } else {
        // timed
        bool timedout = false;
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;
        while (!timedout && condition_->anonymous_value == 0) {
            if (pthread_cond_timedwait(&(condition_->anonymous_cond), &(condition_->anonymous_mutex), &time_out) == ETIMEDOUT) {
                timedout = true;
            }
        }
        if (!timedout) {
            condition_->anonymous_value--;
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(&(semaphore_->anonymous_mutex));

    return result;
}
int semaphore_post_anonymous(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(semaphore_->anonymous_mutex));

    // signal the pthread cond
    int result = -1;
    semaphore_->anonymous_value++;
    if (pthread_cond_signal(&(semaphore_->anonymous_cond)) == 0) {
        result = 0;
    }

    // release the pthread mutex
    pthread_mutex_unlock(&(semaphore_->anonymous_mutex));

    return result;
}
int semaphore_get_anonymous(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(&(semaphore_->anonymous_mutex));

    // get value
    int result = semaphore_->anonymous_value;

    // release the pthread mutex
    pthread_mutex_unlock(&(semaphore_->anonymous_mutex));

    return result;
}
int semaphore_wait_named(struct Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(semaphore_->named_mutex->address(semaphore_->named_mutex));

    // wait the pthread cond
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity
        while (*((int*)semaphore_->named_value->address(semaphore_->named_value)) == 0) {
            pthread_cond_wait(semaphore_->named_cond->address(semaphore_->named_cond), semaphore_->named_mutex->address(semaphore_->named_mutex));
        }
        *((int*)semaphore_->named_value->address(semaphore_->named_value))--;
        result = 0;
    } else {
        // timed
        bool timedout = false;
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;
        while (!timedout && *((int*)semaphore_->named_value->address(semaphore_->named_value)) == 0) {
            if (pthread_cond_timedwait(semaphore_->named_cond->address(semaphore_->named_cond), semaphore_->named_mutex->address(semaphore_->named_mutex), &time_out) == ETIMEDOUT) {
                timedout = true;
            }
        }
        if (!timedout) {
            *((int*)semaphore_->named_value->address(semaphore_->named_value))--;
            result = 0;
        }
    }

    // release the pthread mutex
    pthread_mutex_unlock(semaphore_->named_mutex->address(semaphore_->named_mutex));

    return result;
}
int semaphore_post_named(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(semaphore_->named_mutex->address(semaphore_->named_mutex));

    // signal the pthread cond
    int result = -1;
    *((int*)semaphore_->named_value->address(semaphore_->named_value))++;
    if (pthread_cond_signal(semaphore_->named_cond->address(semaphore_->named_cond)) == 0) {
        result = 0;
    }

    // release the pthread mutex
    pthread_mutex_unlock(semaphore_->named_mutex->address(semaphore_->named_mutex));

    return result;
}
int semaphore_get_named(struct Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // aquire the pthread mutex
    pthread_mutex_lock(semaphore_->named_mutex->address(semaphore_->named_mutex));

    // get value
    int result = *((int*)semaphore_->named_value->address(semaphore_->named_value));

    // release the pthread mutex
    pthread_mutex_unlock(semaphore_->named_mutex->address(semaphore_->named_mutex));

    return result;
}

Semaphore* semaphore_new(char* name, int value) {
    struct Semaphore_* semaphore_ = heap_alloc(sizeof(struct Semaphore_));

    if (name == NULL) {
        // init private methods
        semaphore_->self.wait = semaphore_wait_anonymous;
        semaphore_->self.post = semaphore_post_anonymous;
        semaphore_->self.get = semaphore_get_anonymous;

        // init internal mutex
        semaphore_->named_mutex = NULL;
        pthread_mutex_init(&(semaphore_->anonymous_mutex), NULL);

        // init internal cond
        semaphore_->named_cond = NULL;
        pthread_cond_init(&(semaphore_->anonymous_cond), NULL);

        // init internal value
        semaphore_->named_value = NULL;
        semaphore_->anonymous_value = value;
    } else {
        // init private methods
        semaphore_->self.wait = semaphore_wait_named;
        semaphore_->self.post = semaphore_post_named;
        semaphore_->self.get = semaphore_get_named;

        // init internal share mutex
        String* mutex_name = string_new_concat(name, "/mutex");
        semaphore_->named_mutex = share_new(mutex_name, sizeof(pthread_mutex_t));
        string_free(mutex_name);

        // init internal share cond
        String* cond_name = string_new_concat(name, "/cond");
        semaphore_->named_cond = share_new(cond_name, sizeof(pthread_cond_t));
        string_free(cond_name);

        // init internal share value
        String* value_name = string_new_concat(name, "/value");
        semaphore_->named_value = share_new(value_name, sizeof(int));
        string_free(value_name);

        // if share mutex not created before, init it
        if (semaphore_->named_mutex->connections(semaphore_->named_mutex) <= 1) {
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, 1);
            pthread_mutex_init(semaphore_->named_mutex->address(semaphore_->named_mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }

        // if share cond not created before, init it
        if (semaphore_->named_cond->connections(semaphore_->named_cond) <= 1) {
            pthread_condattr_t cattr;
            pthread_condattr_init(&cattr);
            pthread_condattr_setpshared(&cattr, 1);
            pthread_cond_init(semaphore_->named_cond->address(semaphore_->named_cond), &cattr);
            pthread_condattr_destroy(&cattr);
        }

        // if share value not created before, init it
        if (semaphore_->named_value->connections(semaphore_->named_value) <= 1) {
            *((int*)semaphore_->named_value->address(semaphore_->named_value)) = value;
        }
    }

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    if (semaphore_->named_mutex == NULL) {
        // destroy internal mutex, cond, value
        pthread_mutex_destroy(&(semaphore_->anonymous_mutex));
        pthread_cond_destroy(&(semaphore_->anonymous_cond));
        semaphore_->anonymous_value = -1;
    } else {
        // destroy internal share mutex
        if (semaphore_->named_mutex->connections(semaphore_->named_mutex) <= 1) {
            pthread_mutex_destroy(semaphore_->named_mutex->address(semaphore_->named_mutex));
        }
        share_free(semaphore_->named_mutex);

        // destroy internal share cond
        if (semaphore_->named_cond->connections(semaphore_->named_cond) <= 1) {
            pthread_cond_destroy(semaphore_->named_cond->address(semaphore_->named_cond));
        }
        share_free(semaphore_->named_cond);

        // destroy internal share value
        if (semaphore_->named_value->connections(semaphore_->named_value) <= 1) {
            semaphore_->anonymous_value = -1;
        }
        share_free(semaphore_->named_value);
    }

    heap_free(semaphore_);
}

#endif