#include <low/Monitor.h>

#if defined(APP_WINDOWS)

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
    MutexLock* critical_lock;
    Semaphore* sleep_semaphore;
    Share* sleepers_share;
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

    // get sleepers address
    int* sleepers = (int*) monitor_->sleepers_share->vtable->address(monitor_->sleepers_share);

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
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get sleepers address
    int* sleepers = (int*) monitor_->sleepers_share->vtable->address(monitor_->sleepers_share);

    // lock the critical mutex
    monitor_->critical_lock->vtable->lock(monitor_->critical_lock, UINT_64_MAX);

    // signal
    int result = 0;
    if (*sleepers > 0) {
        // signal to wake up
        if(monitor_->sleep_semaphore->vtable->post(monitor_->sleep_semaphore) == 0){
            // reduce sleepers
            (*sleepers)--;
        }else{
            result = -1;
        }
    }

    // unlock the critical mutex
    monitor_->critical_lock->vtable->unlock(monitor_->critical_lock);

    return result;
}
int monitor_notify_all(Monitor* self) {
    struct Monitor_* monitor_ = (struct Monitor_*)self;

    // get sleepers address
    int* sleepers = (int*) monitor_->sleepers_share->vtable->address(monitor_->sleepers_share);

    // lock the critical mutex
    monitor_->critical_lock->vtable->lock(monitor_->critical_lock, UINT_64_MAX);

    // broadcast
    while ((*sleepers) > 0) {
        // signal to wake up
        monitor_->sleep_semaphore->vtable->post(monitor_->sleep_semaphore);

        // reduce sleepers
        (*sleepers)--;
    }

    // unlock the critical mutex
    monitor_->critical_lock->vtable->unlock(monitor_->critical_lock);

    return 0;
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
    monitor_->critical_lock = NULL;
    monitor_->sleep_semaphore = NULL;
    monitor_->sleepers_share = NULL;

    return (Monitor*)monitor_;
}
void monitor_free(Monitor* monitor) {
    struct Monitor_* monitor_ = (struct Monitor_*)monitor;

    // free private data
    if(monitor_->critical_lock != NULL){
        mutexlock_free(monitor_->critical_lock);
    }
    if(monitor_->sleep_semaphore != NULL){
        semaphore_free(monitor_->sleep_semaphore);
    }
    if(monitor_->sleepers_share != NULL){
        share_free(monitor_->sleepers_share);
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

        // open share critical mutexlock
        String* monitor_critical_name = string_new_printf("%s_monitor_critical", name);
        monitor_->critical_lock = mutexlock_new_object(monitor_critical_name->vtable->value(monitor_critical_name));
        string_free(monitor_critical_name);

        // open share sleep semaphore
        String* monitor_sleep_name = string_new_printf("%s_monitor_sleep", name);
        monitor_->sleep_semaphore = semaphore_new_object(monitor_sleep_name->vtable->value(monitor_sleep_name), 0);
        string_free(monitor_sleep_name);

        // open share sleepers share
        String* monitor_sleepers_name = string_new_printf("%s_monitor_sleepers", name);
        monitor_->sleepers_share = share_new_object(monitor_sleepers_name->vtable->value(monitor_sleepers_name), sizeof(int), 0);
        string_free(monitor_sleepers_name);

        // if share connections is 1, init share
        if(monitor_->sleepers_share->vtable->connections(monitor_->sleepers_share) <= 1){
            // get sleepers address
            int* sleepers = (int*) monitor_->sleepers_share->vtable->address(monitor_->sleepers_share);

            // init readers
            (*sleepers) = 0;
        }

        // try unlock critical
        if (critical != NULL) {
            critical->vtable->unlock(critical);
        }
    } else {
        // open critical mutexlock
        monitor_->critical_lock = mutexlock_new_object(NULL);

        // open sleep semaphore
        monitor_->sleep_semaphore = semaphore_new_object(NULL, 0);

        // open sleepers share
        monitor_->sleepers_share = share_new_object(NULL, sizeof(int), 0);

        // if connections is 1, init share
        if(monitor_->sleepers_share->vtable->connections(monitor_->sleepers_share) <= 1){
            // get sleepers address
            int* sleepers = (int*) monitor_->sleepers_share->vtable->address(monitor_->sleepers_share);

            // init readers
            (*sleepers) = 0;
        }
    }

    return (Monitor*)monitor_;
}

#endif
