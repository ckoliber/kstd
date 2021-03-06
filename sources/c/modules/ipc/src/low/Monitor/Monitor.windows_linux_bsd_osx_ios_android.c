#include <low/Monitor.h>

#if defined(APP_WINDOWS) || defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/ReentrantLock.h>
#include <low/Semaphore.h>
#include <low/MutexLock.h>
#include <low/String.h>
#include <low/Heap.h>

struct Monitor_ {
    // self public object
    Monitor self;

    // constructor data

    // private data
    Share* share;
    MutexLock* critical_lock;
    Semaphore* sleep_semaphore;
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
    /*  Algorithm:
     *
     *      lock(critical)
     *      sleepers++;
     *      unlock(critical)
     *      if(wait(semaphore) == 0){
     *          return 0;
     *      }else{
     *          return -1;
     *      }
     *
     */
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get sleepers address
    int* sleepers = monitor_->share->vtable->address(monitor_->share);

    // lock the critical mutex
    monitor_->critical_lock->vtable->lock(monitor_->critical_lock, UINT_64_MAX);

    // critical section
    (*sleepers)++;

    // unlock the critical mutex
    monitor_->critical_lock->vtable->unlock(monitor_->critical_lock);

    // goto sleep wait
    if (monitor_->sleep_semaphore->vtable->wait(monitor_->sleep_semaphore, timeout) == 0) {
        return 0;
    }

    return -1;
}
int monitor_notify(Monitor* self) {
    /*  Algorithm:
     *
     *      lock(critical)
     *      result = -1
     *      if(sleepers > 0){
     *          post(semaphore)
     *          sleepers--
     *          result = 0
     *      }
     *      unlock(critical)
     *      return result
     *
     */
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get sleepers address
    int* sleepers = monitor_->share->vtable->address(monitor_->share);

    // lock the critical mutex
    monitor_->critical_lock->vtable->lock(monitor_->critical_lock, UINT_64_MAX);

    // signal
    int result = -1;
    if ((*sleepers) > 0) {
        // signal to wake up
        monitor_->sleep_semaphore->vtable->post(monitor_->sleep_semaphore);

        // reduce sleepers
        (*sleepers)--;

        result = 0;
    }

    // unlock the critical mutex
    monitor_->critical_lock->vtable->unlock(monitor_->critical_lock);

    return result;
}
int monitor_notify_all(Monitor* self) {
    /*  Algorithm:
     *
     *      lock(critical)
     *      result = -1
     *      while(sleepers > 0){
     *          post(semaphore)
     *          sleepers--
     *          result = 0
     *      }
     *      unlock(critical)
     *      return result
     *
     */
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get sleepers address
    int* sleepers = monitor_->share->vtable->address(monitor_->share);

    // lock the critical mutex
    monitor_->critical_lock->vtable->lock(monitor_->critical_lock, UINT_64_MAX);

    // broadcast
    int result = -1;
    while ((*sleepers) > 0) {
        // signal to wake up
        monitor_->sleep_semaphore->vtable->post(monitor_->sleep_semaphore);

        // reduce sleepers
        (*sleepers)--;

        result = 0;
    }

    // unlock the critical mutex
    monitor_->critical_lock->vtable->unlock(monitor_->critical_lock);

    return result;
}

// object allocation and deallocation operators
void monitor_init() {
    // init vtable
    monitor_vtable = heap_alloc(sizeof(Monitor_VTable));
    monitor_vtable->wait = monitor_wait;
    monitor_vtable->notify = monitor_notify;
    monitor_vtable->notify_all = monitor_notify_all;
}
Monitor* monitor_new() {
    struct Monitor_* monitor_ = heap_alloc(sizeof(struct Monitor_));

    // set vtable
    monitor_->self.vtable = monitor_vtable;

    // set constructor data

    // set private data
    monitor_->share = NULL;
    monitor_->critical_lock = NULL;
    monitor_->sleep_semaphore = NULL;

    return (Monitor*)monitor_;
}
void monitor_free(Monitor* monitor) {
    struct Monitor_* monitor_ = (struct Monitor_*)monitor;

    // free private data
    if(monitor_->share != NULL){
        share_free(monitor_->share);
    }
    if(monitor_->critical_lock != NULL){
        mutexlock_free(monitor_->critical_lock);
    }
    if(monitor_->sleep_semaphore != NULL){
        semaphore_free(monitor_->sleep_semaphore);
    }

    // free constructor data

    // free self
    heap_free(monitor_);
}
Monitor* monitor_new_anonymous(){
    struct Monitor_* monitor_ = (struct Monitor_*)monitor_new();

    // set constructor data

    // set private data
    // open sleepers share
    monitor_->share = share_new_anonymous(sizeof(int), 0);

    // if connections is 1, init share
    if(monitor_->share->vtable->connections(monitor_->share) <= 1){
        // get sleepers address
        int* sleepers = monitor_->share->vtable->address(monitor_->share);

        // init readers
        (*sleepers) = 0;
    }

    // open critical mutexlock
    monitor_->critical_lock = mutexlock_new_anonymous();

    // open sleep semaphore
    monitor_->sleep_semaphore = semaphore_new_anonymous(0);

    return (Monitor*)monitor_;
}
Monitor* monitor_new_named(char* name){
    struct Monitor_* monitor_ = (struct Monitor_*)monitor_new();

    // set constructor data

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // open share sleepers share
    String* monitor_sleepers_name = string_new_printf("%s_mo_ss", name);
    monitor_->share = share_new_named(monitor_sleepers_name->vtable->value(monitor_sleepers_name), sizeof(int), 0);
    string_free(monitor_sleepers_name);

    // if share connections is 1, init share
    if(monitor_->share->vtable->connections(monitor_->share) <= 1){
        // get sleepers address
        int* sleepers = monitor_->share->vtable->address(monitor_->share);

        // init readers
        (*sleepers) = 0;
    }

    // open share critical mutexlock
    String* monitor_critical_name = string_new_printf("%s_mo_c", name);
    monitor_->critical_lock = mutexlock_new_named(monitor_critical_name->vtable->value(monitor_critical_name));
    string_free(monitor_critical_name);

    // open share sleep semaphore
    String* monitor_sleep_name = string_new_printf("%s_mo_s", name);
    monitor_->sleep_semaphore = semaphore_new_named(monitor_sleep_name->vtable->value(monitor_sleep_name), 0);
    string_free(monitor_sleep_name);

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Monitor*)monitor_;
}

#endif
