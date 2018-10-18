#include <low/MutexLock.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>

struct MutexLock_ {
    // self public object
    MutexLock self;

    // constructor data

    // private data
    Share* share;
};

struct MutexLock_Memory{
    pthread_mutex_t mutex;
};

// vtable
MutexLock_VTable* mutexlock_vtable;

// link methods
int mutexlock_lock(MutexLock* self, uint_64 timeout);
int mutexlock_unlock(MutexLock* self);

// implement methods
// vtable operators
int mutexlock_lock(MutexLock* self, uint_64 timeout) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)self;

    // get memory address
    struct MutexLock_Memory* memory = (struct MutexLock_Memory*) mutexlock_->share->vtable->address(mutexlock_->share);

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
int mutexlock_unlock(MutexLock* self) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)self;

    // get memory address
    struct MutexLock_Memory* memory = (struct MutexLock_Memory*) mutexlock_->share->vtable->address(mutexlock_->share);

    // unlock the pthread mutex
    if (pthread_mutex_unlock(&(memory->mutex)) == 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void mutexlock_init() {
    // init vtable
    mutexlock_vtable = (MutexLock_VTable*) heap_alloc(sizeof(MutexLock_VTable));
    mutexlock_vtable->lock = mutexlock_lock;
    mutexlock_vtable->unlock = mutexlock_unlock;
}
MutexLock* mutexlock_new() {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*) heap_alloc(sizeof(struct MutexLock_));

    // set vtable
    mutexlock_->self.vtable = mutexlock_vtable;

    // set constructor data

    // set private data
    mutexlock_->share = NULL;

    return (MutexLock*)mutexlock_;
}
void mutexlock_free(MutexLock* mutexlock) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)mutexlock;

    // free private data
    if (mutexlock_->share != NULL) {
        // if share connections is 1, destroy
        if(mutexlock_->share->vtable->connections(mutexlock_->share) <= 1){
            struct MutexLock_Memory* memory = (struct MutexLock_Memory*) mutexlock_->share->vtable->address(mutexlock_->share);

            // destroy
            pthread_mutex_destroy(&(memory->mutex));
        }

        share_free(mutexlock_->share);
    }

    // free constructor data

    // free self
    heap_free((uint_8*) mutexlock_);
}
MutexLock* mutexlock_new_object(char* name) {
    struct MutexLock_* mutexlock_ = (struct MutexLock_*)mutexlock_new();

    // set constructor data

    // set private data
    if (name != NULL) {
        // try lock critical
        if (critical != NULL) {
            critical->vtable->lock(critical, UINT_64_MAX);
        }

        // open share errorcheck lock
        String* mutexlock_name = string_new_printf("%s_mutexlock", name);
        mutexlock_->share = share_new_object(mutexlock_name->vtable->value(mutexlock_name), sizeof(struct MutexLock_Memory), 0);
        string_free(mutexlock_name);

        // if share connections is 1, init share
        if(mutexlock_->share->vtable->connections(mutexlock_->share) <= 1){
            // get memory address
            struct MutexLock_Memory* memory = (struct MutexLock_Memory*) mutexlock_->share->vtable->address(mutexlock_->share);

            // init mutex
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }

        // try unlock critical
        if (critical != NULL) {
            critical->vtable->unlock(critical);
        }
    } else {
        // open errorcheck lock
        mutexlock_->share = share_new_object(NULL, sizeof(struct MutexLock_Memory), 0);

        // if share connections is 1, init share
        if(mutexlock_->share->vtable->connections(mutexlock_->share) <= 1){
            // get memory address
            struct MutexLock_Memory* memory = (struct MutexLock_Memory*) mutexlock_->share->vtable->address(mutexlock_->share);

            // init mutex
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_PRIVATE);
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);
        }
    }

    return (MutexLock*)mutexlock_;
}

#endif
