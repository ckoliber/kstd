#include <low/Monitor.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>

struct Monitor_ {
    // self public object
    Monitor self;

    // constructor data

    // private data
    Share* share;
};

struct Monitor_Memory{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

// vtable
Monitor_VTable* monitor_vtable;

// link methods
int monitor_wait(Monitor* self, uint_64 timeout);
int monitor_notify(Monitor* self);
int monitor_notify_all(Monitor* self);

// implement methods
// vtable operators
int monitor_wait(Monitor* self, uint_64 timeout) {
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get memory address
    struct Monitor_Memory* memory = (struct Monitor_Memory*) monitor_->share->vtable->address(monitor_->share);

    // wait the pthread cond
    int result = -1;
    if(pthread_mutex_lock(&(memory->mutex)) == 0){
        if(timeout == UINT_64_MAX){
            // infinity

            // wait
            if (pthread_cond_wait(&(memory->cond), &(memory->mutex)) == 0) {
                result = 0;
            }
        }else{
            // timed

            // get time_out
            struct timeval time_now;
            struct timespec time_out;
            gettimeofday(&time_now, NULL);
            time_out.tv_sec = time_now.tv_sec;
            time_out.tv_nsec = time_now.tv_usec * 1000;
            time_out.tv_sec += timeout / 1000;
            time_out.tv_nsec += (timeout % 1000) * 1000000;

            // timed wait the cond
            if (pthread_cond_timedwait(&(memory->cond), &(memory->mutex), &time_out) == 0) {
                result = 0;
            }
        }

        // unlock mutex
        pthread_mutex_unlock(&(memory->mutex));
    }

    return result;
}
int monitor_notify(Monitor* self) {
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get memory address
    struct Monitor_Memory* memory = (struct Monitor_Memory*) monitor_->share->vtable->address(monitor_->share);

    // notify the pthread cond
    int result = -1;
    if(pthread_mutex_lock(&(memory->mutex)) == 0) {
        // notify
        if (pthread_cond_signal(&(memory->cond)) == 0) {
            result = 0;
        }

        // unlock mutex
        pthread_mutex_unlock(&(memory->mutex));
    }

    return result;
}
int monitor_notify_all(Monitor* self) {
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get memory address
    struct Monitor_Memory* memory = (struct Monitor_Memory*) monitor_->share->vtable->address(monitor_->share);

    // notify all the pthread cond
    int result = -1;
    if(pthread_mutex_lock(&(memory->mutex)) == 0) {
        // notify all
        if (pthread_cond_broadcast(&(memory->cond)) == 0) {
            result = 0;
        }

        // unlock mutex
        pthread_mutex_unlock(&(memory->mutex));
    }

    return result;
}

// object allocation and deallocation operators
void monitor_init() {
    // init vtable
    monitor_vtable = (Monitor_VTable*) heap_alloc(sizeof(Monitor_VTable));
    monitor_vtable->wait = monitor_wait;
    monitor_vtable->notify = monitor_notify;
    monitor_vtable->notify_all = monitor_notify_all;
}
Monitor* monitor_new() {
    struct Monitor_* monitor_ = (struct Monitor_*) heap_alloc(sizeof(struct Monitor_));

    // set vtable
    monitor_->self.vtable = monitor_vtable;

    // set constructor data

    // set private data
    monitor_->share = NULL;

    return (Monitor*)monitor_;
}
void monitor_free(Monitor* monitor) {
    struct Monitor_* monitor_ = (struct Monitor_*)monitor;

    // free private data
    if (monitor_->share != NULL) {
        // if share connections is 1, destroy
        if(monitor_->share->vtable->connections(monitor_->share) <= 1){
            struct Monitor_Memory* memory = (struct Monitor_Memory*) monitor_->share->vtable->address(monitor_->share);

            // destroy
            pthread_mutex_destroy(&(memory->mutex));
            pthread_cond_destroy(&(memory->cond));
        }

        share_free(monitor_->share);
    }

    // free constructor data

    // free self
    heap_free((uint_8*) monitor_);
}
Monitor* monitor_new_object(char* name) {
    struct Monitor_* monitor_ = (struct Monitor_*)monitor_new();

    // set constructor data

    // set private data
    if (name != NULL) {
        // try lock critical
        if (critical != NULL) {
            critical->vtable->lock(critical, UINT_64_MAX);
        }

        // open share errorcheck lock
        String* monitor_name = string_new_printf("%s_monitor", name);
        monitor_->share = share_new_object(monitor_name->vtable->value(monitor_name), sizeof(struct Monitor_Memory), 0);
        string_free(monitor_name);

        // if share connections is 1, init share
        if(monitor_->share->vtable->connections(monitor_->share) <= 1){
            // get memory address
            struct Monitor_Memory* memory = (struct Monitor_Memory*) monitor_->share->vtable->address(monitor_->share);

            // init mutex
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_SHARED);
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);

            // init cond
            pthread_condattr_t cattr;
            pthread_condattr_init(&cattr);
            pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_SHARED);
            pthread_cond_init(&(memory->cond), &cattr);
            pthread_condattr_destroy(&cattr);
        }

        // try unlock critical
        if (critical != NULL) {
            critical->vtable->unlock(critical);
        }
    } else {
        // open errorcheck lock
        monitor_->share = share_new_object(NULL, sizeof(struct Monitor_Memory), 0);

        // if share connections is 1, init share
        if(monitor_->share->vtable->connections(monitor_->share) <= 1){
            // get memory address
            struct Monitor_Memory* memory = (struct Monitor_Memory*) monitor_->share->vtable->address(monitor_->share);

            // init mutex
            pthread_mutexattr_t mattr;
            pthread_mutexattr_init(&mattr);
            pthread_mutexattr_setpshared(&mattr, PTHREAD_PROCESS_PRIVATE);
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->mutex), &mattr);
            pthread_mutexattr_destroy(&mattr);

            // init cond
            pthread_condattr_t cattr;
            pthread_condattr_init(&cattr);
            pthread_condattr_setpshared(&cattr, PTHREAD_PROCESS_PRIVATE);
            pthread_cond_init(&(memory->cond), &cattr);
            pthread_condattr_destroy(&cattr);
        }
    }

    return (Monitor*)monitor_;
}

#endif
