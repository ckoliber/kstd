#include <low/ReentrantLock.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>

struct ReentrantLock_ {
    // self public object
    ReentrantLock self;

    // constructor data

    // private data
    Share* share;
};

struct ReentrantLock_Memory{
    pthread_mutex_t mutex;
};

// vtable
ReentrantLock_VTable* reentrantlock_vtable;

// link methods
int reentrantlock_lock(ReentrantLock* self, uint_64 timeout);
int reentrantlock_unlock(ReentrantLock* self);

// implement methods
// vtable operators
int reentrantlock_lock(ReentrantLock* self, uint_64 timeout) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)self;

    // get memory address
    struct ReentrantLock_Memory* memory = (struct ReentrantLock_Memory*) reentrantlock_->share->vtable->address(reentrantlock_->share);

    // lock the pthread mutex
    if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_mutex_lock(&(memory->mutex)) == 0) {
            return 0;
        }
    } else if(timeout > 0){
        // timed

        // get time_out
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;

        // timed lock
        if (pthread_mutex_timedlock(&(memory->mutex), &time_out) == 0) {
            return 0;
        }
    }else{
        // try
        if (pthread_mutex_trylock(&(memory->mutex)) == 0) {
            return 0;
        }
    }

    return -1;
}
int reentrantlock_unlock(ReentrantLock* self) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)self;

    // get memory address
    struct ReentrantLock_Memory* memory = (struct ReentrantLock_Memory*) reentrantlock_->share->vtable->address(reentrantlock_->share);

    // unlock the pthread mutex
    if (pthread_mutex_unlock(&(memory->mutex)) == 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void reentrantlock_init() {
    // init vtable
    reentrantlock_vtable = (ReentrantLock_VTable*) heap_alloc(sizeof(ReentrantLock_VTable));
    reentrantlock_vtable->lock = reentrantlock_lock;
    reentrantlock_vtable->unlock = reentrantlock_unlock;
}
ReentrantLock* reentrantlock_new() {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*) heap_alloc(sizeof(struct ReentrantLock_));

    // set vtable
    reentrantlock_->self.vtable = reentrantlock_vtable;

    // set constructor data

    // set private data
    reentrantlock_->share = NULL;

    return (ReentrantLock*)reentrantlock_;
}
void reentrantlock_free(ReentrantLock* reentrantlock) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)reentrantlock;

    // free private data
    if (reentrantlock_->share != NULL) {
        // if share connections is 1, destroy
        if(reentrantlock_->share->vtable->connections(reentrantlock_->share) <= 1){
            struct ReentrantLock_Memory* memory = (struct ReentrantLock_Memory*) reentrantlock_->share->vtable->address(reentrantlock_->share);

            // destroy
            pthread_mutex_destroy(&(memory->mutex));
        }

        share_free(reentrantlock_->share);
    }

    // free constructor data

    // free self
    heap_free((uint_8*) reentrantlock_);
}
ReentrantLock* reentrantlock_new_object(char* name) {
    struct ReentrantLock_* reentrantlock_ = (struct ReentrantLock_*)reentrantlock_new();

    // set constructor data

    // set private data
    if (name != NULL) {
        // try lock critical
        if (critical != NULL) {
            critical->vtable->lock(critical, UINT_64_MAX);
        }

        // open share errorcheck lock
        String* reentrantlock_name = string_new_printf("%s_reentrantlock", name);
        reentrantlock_->share = share_new_object(reentrantlock_name->vtable->value(reentrantlock_name), sizeof(struct ReentrantLock_Memory), 0);
        string_free(reentrantlock_name);

        // if share connections is 1, init share
        if(reentrantlock_->share->vtable->connections(reentrantlock_->share) <= 1){
            // get memory address
            struct ReentrantLock_Memory* memory = (struct ReentrantLock_Memory*) reentrantlock_->share->vtable->address(reentrantlock_->share);

            // init mutex
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&(memory->mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }

        // try unlock critical
        if (critical != NULL) {
            critical->vtable->unlock(critical);
        }
    } else {
        // open errorcheck lock
        reentrantlock_->share = share_new_object(NULL, sizeof(struct ReentrantLock_Memory), 0);

        // if share connections is 1, init share
        if(reentrantlock_->share->vtable->connections(reentrantlock_->share) <= 1){
            // get memory address
            struct ReentrantLock_Memory* memory = (struct ReentrantLock_Memory*) reentrantlock_->share->vtable->address(reentrantlock_->share);

            // init mutex
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_PRIVATE);
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&(memory->mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }
    }

    return (ReentrantLock*)reentrantlock_;
}

#endif
