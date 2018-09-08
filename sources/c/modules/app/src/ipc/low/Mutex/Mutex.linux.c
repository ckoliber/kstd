#include <ipc/low/Mutex.h>

#if defined(APP_LINUX)

#include <dsa/low/String.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <memory/low/Share.h>
#include <pthread.h>

struct Mutex_ {
    struct Mutex self;
    pthread_mutex_t anonymous_mutex;
    Share* named_mutex;
};

// link methods
int mutex_acquire_anonymous(struct Mutex* self, uint_64 timeout);
int mutex_release_anonymous(struct Mutex* self);
int mutex_acquire_named(struct Mutex* self, uint_64 timeout);
int mutex_release_named(struct Mutex* self);

// implement methods
int mutex_acquire_anonymous(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // aquire the pthread mutex
    int result = -1;
    if (timeout == 0) {
        // try
        if (pthread_mutex_trylock(&(mutex_->anonymous_mutex)) == 0) {
            result = 0;
        }
    } else if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_mutex_lock(&(mutex_->anonymous_mutex)) == 0) {
            result = 0;
        }
    } else {
        // timed
        uint_64 time = time_epochmillis();

        // try lock until timeout
        while ((time_epochmillis() - time) <= timeout) {
            // try
            if (pthread_mutex_trylock(&(mutex_->anonymous_mutex)) == 0) {
                result = 0;
                break;
            }
        }
    }

    return result;
}
int mutex_release_anonymous(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // release the pthread mutex
    int result = -1;
    if (pthread_mutex_unlock(&(mutex_->anonymous_mutex)) == 0) {
        result = 0;
    }

    return result;
}
int mutex_acquire_named(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // aquire the pthread share mutex
    int result = -1;
    if (timeout == 0) {
        // try
        if (pthread_mutex_trylock(mutex_->named_mutex->address(mutex_->named_mutex)) == 0) {
            result = 0;
        }
    } else if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_mutex_lock(mutex_->named_mutex->address(mutex_->named_mutex)) == 0) {
            result = 0;
        }
    } else {
        // timed
        uint_64 time = time_epochmillis();

        // try lock until timeout
        while ((time_epochmillis() - time) <= timeout) {
            // try
            if (pthread_mutex_trylock(mutex_->named_mutex->address(mutex_->named_mutex)) == 0) {
                result = 0;
                break;
            }
        }
    }

    return result;
}
int mutex_release_named(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // release the pthread share mutex
    int result = -1;
    if (pthread_mutex_unlock(mutex_->named_mutex->address(mutex_->named_mutex)) == 0) {
        result = 0;
    }

    return result;
}

Mutex* mutex_new(char* name) {
    struct Mutex_* mutex_ = heap_alloc(sizeof(struct Mutex_));

    if (name == NULL) {
        // init private methods
        mutex_->self.acquire = mutex_acquire_anonymous;
        mutex_->self.release = mutex_release_anonymous;

        // init internal mutex
        mutex_->named_mutex = NULL;
        pthread_mutex_init(&(mutex_->anonymous_mutex), NULL);
    } else {
        // init private methods
        mutex_->self.acquire = mutex_acquire_named;
        mutex_->self.release = mutex_release_named;

        // init internal share mutex
        String* mutex_name = string_new_concat(name, "/mutex");
        mutex_->named_mutex = share_new(mutex_name, sizeof(pthread_mutex_t));
        string_free(mutex_name);

        // if share mutex not created before, init it
        if (mutex_->named_mutex->connections(mutex_->named_mutex) <= 1) {
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, 1);
            pthread_mutex_init(mutex_->named_mutex->address(mutex_->named_mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }
    }

    return (Mutex*)mutex_;
}
void mutex_free(Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    if (mutex_->named_mutex == NULL) {
        // destroy internal mutex
        pthread_mutex_destroy(&(mutex_->anonymous_mutex));
    } else {
        // destroy internal share mutex
        if (mutex_->named_mutex->connections(mutex_->named_mutex) <= 1) {
            pthread_mutex_destroy(mutex_->named_mutex->address(mutex_->named_mutex));
        }
        share_free(mutex_->named_mutex);
    }

    heap_free(mutex_);
}

#endif