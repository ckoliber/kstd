#include <low/Latch.h>

#if defined(APP_WINDOWS) || defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/String.h>
#include <low/Heap.h>
#include <low/ReentrantLock.h>
#include <low/MutexLock.h>
#include <low/Semaphore.h>

struct Latch_ {
    // self public object
    Latch self;

    // constructor data

    // private data
    Share* share;
    MutexLock* critical_lock;
    Semaphore* latch_semaphore;
};

struct Latch_Memory{
    int value;
    int downs;
};

// vtable
Latch_VTable* latch_vtable;

// link methods
int latch_wait(Latch* self, uint_64 timeout);
int latch_down(Latch* self);
int latch_get(Latch* self);

// implement methods
// vtable operators
int latch_wait(Latch* self, uint_64 timeout){
    /*  Algorithm:
     *
     *      if(wait(semaphore) != 0){
     *          return -1
     *      }
     *      return 0
     *
     */
    struct Latch_* latch_ = (struct Latch_*)self;

    // wait on semaphore
    if(latch_->latch_semaphore->vtable->wait(latch_->latch_semaphore, timeout) != 0){
        return -1;
    }

    return 0;
}
int latch_down(Latch* self){
    /*  Algorithm:
     *
     *      lock(critical)
     *      downs = (downs + 1) % value
     *      if(downs == 0){
     *          post(semaphore)
     *      }
     *      unlock(critical)
     *
     *      return 0
     *
     */
    struct Latch_* latch_ = (struct Latch_*)self;

    // get memory address
    struct Latch_Memory* memory = latch_->share->vtable->address(latch_->share);

    // safe increase downs
    latch_->critical_lock->vtable->lock(latch_->critical_lock, UINT_64_MAX);

    // increase downs
    memory->downs = (memory->downs + 1) % memory->value;

    // check downs are full then post semaphore
    if(memory->downs == 0){
        latch_->latch_semaphore->vtable->post(latch_->latch_semaphore);
    }

    latch_->critical_lock->vtable->unlock(latch_->critical_lock);

    return 0;
}
int latch_get(Latch* self){
    struct Latch_* latch_ = (struct Latch_*)self;

    // get memory address
    struct Latch_Memory* memory = latch_->share->vtable->address(latch_->share);

    // get value
    int result = memory->downs;

    return result;
}

// object allocation and deallocation operators
void latch_init() {
    // init vtable
    latch_vtable = heap_alloc(sizeof(Latch_VTable));
    latch_vtable->wait = latch_wait;
    latch_vtable->down = latch_down;
    latch_vtable->get = latch_get;
}
Latch* latch_new() {
    struct Latch_* latch_ = heap_alloc(sizeof(struct Latch_));

    // set vtable
    latch_->self.vtable = latch_vtable;

    // set constructor data

    // set private data
    latch_->share = NULL;
    latch_->critical_lock = NULL;
    latch_->latch_semaphore = NULL;

    return (Latch*)latch_;
}
void latch_free(Latch* latch) {
    struct Latch_* latch_ = (struct Latch_*)latch;

    // free private data
    if(latch_->share != NULL){
        share_free(latch_->share);
    }
    if(latch_->critical_lock != NULL){
        mutexlock_free(latch_->critical_lock);
    }
    if(latch_->latch_semaphore != NULL){
        semaphore_free(latch_->latch_semaphore);
    }

    // free constructor data

    // free self
    heap_free(latch_);
}
Latch* latch_new_anonymous(int value){
    struct Latch_* latch_ = (struct Latch_*)latch_new();

    // set constructor data

    // set private data
    // open memory share
    latch_->share = share_new_anonymous(sizeof(struct Latch_Memory), 0);

    // if connections is 1, init share
    if(latch_->share->vtable->connections(latch_->share) <= 1){
        // get memory address
        struct Latch_Memory* memory = latch_->share->vtable->address(latch_->share);

        // init value
        memory->value = value;

        // init downs
        memory->downs = 0;
    }

    // open critical mutexlock
    latch_->critical_lock = mutexlock_new_anonymous();

    // open latch semaphore
    latch_->latch_semaphore = semaphore_new_anonymous(0);

    return (Latch*)latch_;
}
Latch* latch_new_named(char* name, int value){
    struct Latch_* latch_ = (struct Latch_*)latch_new();

    // set constructor data

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // open share memory share
    String* latch_memory_name = string_new_printf("%s_la_sm", name);
    latch_->share = share_new_named(latch_memory_name->vtable->value(latch_memory_name), sizeof(struct Latch_Memory), 0);
    string_free(latch_memory_name);

    // if share connections is 1, init share
    if(latch_->share->vtable->connections(latch_->share) <= 1){
        // get memory address
        struct Latch_Memory* memory = latch_->share->vtable->address(latch_->share);

        // init value
        memory->value = value;

        // init downs
        memory->downs = 0;
    }

    // open share critical mutexlock
    String* latch_critical_name = string_new_printf("%s_la_c", name);
    latch_->critical_lock = mutexlock_new_named(latch_critical_name->vtable->value(latch_critical_name));
    string_free(latch_critical_name);

    // open share latch semaphore
    String* latch_latch_name = string_new_printf("%s_la_s", name);
    latch_->latch_semaphore = semaphore_new_named(latch_latch_name->vtable->value(latch_latch_name), 0);
    string_free(latch_latch_name);

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Latch*)latch_;
}

#endif
