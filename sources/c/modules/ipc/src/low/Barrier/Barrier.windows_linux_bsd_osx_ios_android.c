#include <low/Barrier.h>

#if defined(APP_WINDOWS) || defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/String.h>
#include <low/Heap.h>
#include <low/ReentrantLock.h>
#include <low/MutexLock.h>
#include <low/Semaphore.h>

struct Barrier_ {
    // self public object
    Barrier self;

    // constructor data

    // private data
    Share* share;
    MutexLock* critical_lock;
    Semaphore* barrier_semaphore;
};

struct Barrier_Memory{
    int value;
    int waiters;
};

// vtable
Barrier_VTable* barrier_vtable;

// link methods
int barrier_wait(Barrier* self, uint_64 timeout);
int barrier_get(Barrier* self);

// implement methods
// vtable operators
int barrier_wait(Barrier* self, uint_64 timeout){
    /*  Algorithm:
     *
     *      lock(critical)
     *      waiters = (waiter + 1) % value
     *      if(waiters == 0){
     *          for(cursor in [0..value]){
     *              post(semaphore)
     *          }
     *      }
     *      unlock(critical)
     *
     *      wait(semaphore)
     *      return 0
     *
     */
    struct Barrier_* barrier_ = (struct Barrier_*)self;

    // get memory address
    struct Barrier_Memory* memory = barrier_->share->vtable->address(barrier_->share);

    // safe increase waiters
    barrier_->critical_lock->vtable->lock(barrier_->critical_lock, UINT_64_MAX);

    // increase waiters
    memory->waiters = (memory->waiters + 1) % memory->value;

    // check waiters are full then post number of value to semaphore
    if(memory->waiters == 0){
        for(int cursor = 0 ; cursor < memory->value ; cursor++){
            barrier_->barrier_semaphore->vtable->post(barrier_->barrier_semaphore);
        }
    }

    barrier_->critical_lock->vtable->unlock(barrier_->critical_lock);

    // wait on barrier semaphore
    if(barrier_->barrier_semaphore->vtable->wait(barrier_->barrier_semaphore, timeout) != 0){
        // waiting timeout

        // safe decrease waiters
        barrier_->critical_lock->vtable->lock(barrier_->critical_lock, UINT_64_MAX);
        memory->waiters = (memory->waiters - 1) % memory->value;    // decrease waiters
        barrier_->critical_lock->vtable->unlock(barrier_->critical_lock);

        return -1;
    }

    return 0;
}
int barrier_get(Barrier* self){
    struct Barrier_* barrier_ = (struct Barrier_*)self;

    // get memory address
    struct Barrier_Memory* memory = barrier_->share->vtable->address(barrier_->share);

    // get value
    int result = memory->waiters;

    return result;
}

// object allocation and deallocation operators
void barrier_init() {
    // init vtable
    barrier_vtable = heap_alloc(sizeof(Barrier_VTable));
    barrier_vtable->wait = barrier_wait;
    barrier_vtable->get = barrier_get;
}
Barrier* barrier_new() {
    struct Barrier_* barrier_ = heap_alloc(sizeof(struct Barrier_));

    // set vtable
    barrier_->self.vtable = barrier_vtable;

    // set constructor data

    // set private data
    barrier_->share = NULL;
    barrier_->critical_lock = NULL;
    barrier_->barrier_semaphore = NULL;

    return (Barrier*)barrier_;
}
void barrier_free(Barrier* barrier) {
    struct Barrier_* barrier_ = (struct Barrier_*)barrier;

    // free private data
    if(barrier_->share != NULL){
        share_free(barrier_->share);
    }
    if(barrier_->critical_lock != NULL){
        mutexlock_free(barrier_->critical_lock);
    }
    if(barrier_->barrier_semaphore != NULL){
        semaphore_free(barrier_->barrier_semaphore);
    }

    // free constructor data

    // free self
    heap_free(barrier_);
}
Barrier* barrier_new_anonymous(int value){
    struct Barrier_* barrier_ = (struct Barrier_*)barrier_new();

    // set constructor data

    // set private data
    // open memory share
    barrier_->share = share_new_anonymous(sizeof(struct Barrier_Memory), 0);

    // if connections is 1, init share
    if(barrier_->share->vtable->connections(barrier_->share) <= 1){
        // get memory address
        struct Barrier_Memory* memory = barrier_->share->vtable->address(barrier_->share);

        // init value
        memory->value = value;

        // init waiters
        memory->waiters = 0;
    }

    // open critical mutexlock
    barrier_->critical_lock = mutexlock_new_anonymous();

    // open barrier semaphore
    barrier_->barrier_semaphore = semaphore_new_anonymous(0);

    return (Barrier*)barrier_;
}
Barrier* barrier_new_named(char* name, int value){
    struct Barrier_* barrier_ = (struct Barrier_*)barrier_new();

    // set constructor data

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // open share memory share
    String* barrier_memory_name = string_new_printf("%s_ba_sm", name);
    barrier_->share = share_new_named(barrier_memory_name->vtable->value(barrier_memory_name), sizeof(struct Barrier_Memory), 0);
    string_free(barrier_memory_name);

    // if share connections is 1, init share
    if(barrier_->share->vtable->connections(barrier_->share) <= 1){
        // get memory address
        struct Barrier_Memory* memory = barrier_->share->vtable->address(barrier_->share);

        // init value
        memory->value = value;

        // init waiters
        memory->waiters = 0;
    }

    // open share critical mutexlock
    String* barrier_critical_name = string_new_printf("%s_ba_c", name);
    barrier_->critical_lock = mutexlock_new_named(barrier_critical_name->vtable->value(barrier_critical_name));
    string_free(barrier_critical_name);

    // open share barrier semaphore
    String* barrier_barrier_name = string_new_printf("%s_ba_s", name);
    barrier_->barrier_semaphore = semaphore_new_named(barrier_barrier_name->vtable->value(barrier_barrier_name), 0);
    string_free(barrier_barrier_name);

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Barrier*)barrier_;
}

#endif
