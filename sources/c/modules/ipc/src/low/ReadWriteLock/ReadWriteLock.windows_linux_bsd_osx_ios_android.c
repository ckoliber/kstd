#include <low/ReadWriteLock.h>

#if defined(APP_WINDOWS) || defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/MutexLock.h>
#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>

struct ReadWriteLock_ {
    // self public object
    ReadWriteLock self;

    // constructor data

    // private data
    Share* share;
    MutexLock* critical_lock;
    MutexLock* write_lock;
};

// vtable
ReadWriteLock_VTable* readwritelock_vtable;

// link methods
int readwritelock_read_lock(ReadWriteLock* self, uint_64 timeout);
int readwritelock_read_unlock(ReadWriteLock* self);
int readwritelock_write_lock(ReadWriteLock* self, uint_64 timeout);
int readwritelock_write_unlock(ReadWriteLock* self);

// implement methods
// vtable operators
int readwritelock_read_lock(ReadWriteLock* self, uint_64 timeout) {
    /*  Algorithm:
     *
     *      lock(critical)
     *      if(readers == 0){
     *          if(lock(write) == 0){
     *              readers++;
     *              result = 0;
     *          }
     *      }else{
     *          readers++;
     *          result = 0;
     *      }
     *      unlock(critical)
     *
     */
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)self;

    // get readers count address
    int* readers = readwritelock_->share->vtable->address(readwritelock_->share);

    // lock the write mutex
    int result = -1;

    // begin critical
    if(readwritelock_->critical_lock->vtable->lock(readwritelock_->critical_lock, UINT_64_MAX) == 0){
        // check readers count
        if(*readers == 0){
            // lock the write mutex
            if(readwritelock_->write_lock->vtable->lock(readwritelock_->write_lock, timeout) == 0){
                (*readers)++;
                result = 0;
            }
        }else{
            // write mutex was locked before
            (*readers)++;
            result = 0;
        }

        // end critical
        readwritelock_->critical_lock->vtable->unlock(readwritelock_->critical_lock);
    }

    return result;
}
int readwritelock_read_unlock(ReadWriteLock* self) {
    /*  Algorithm:
     *
     *      lock(critical)
     *      if(readers == 1){
     *          if(unlock(write) == 0){
     *              readers--;
     *              result = 0;
     *          }
     *      }else{
     *          readers--;
     *          result = 0;
     *      }
     *      unlock(critical)
     *
     */
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)self;

    // get readers count address
    int* readers = readwritelock_->share->vtable->address(readwritelock_->share);

    // unlock the write mutex
    int result = -1;

    // begin critical
    if(readwritelock_->critical_lock->vtable->lock(readwritelock_->critical_lock, UINT_64_MAX) == 0){
        // check readers count
        if(*readers == 1){
            // unlock the write mutex
            if(readwritelock_->write_lock->vtable->unlock(readwritelock_->write_lock) == 0){
                (*readers)--;
                result = 0;
            }
        }else{
            // more readers
            (*readers)--;
            result = 0;
        }

        // end critical
        readwritelock_->critical_lock->vtable->unlock(readwritelock_->critical_lock);
    }

    return result;
}
int readwritelock_write_lock(ReadWriteLock* self, uint_64 timeout) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)self;

    // lock the write mutex
    if(readwritelock_->write_lock->vtable->lock(readwritelock_->write_lock, timeout) == 0){
        return 0;
    }

    return -1;
}
int readwritelock_write_unlock(ReadWriteLock* self) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)self;

    // unlock the write mutex
    if(readwritelock_->write_lock->vtable->unlock(readwritelock_->write_lock) == 0){
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void readwritelock_init() {
    // init vtable
    readwritelock_vtable = heap_alloc(sizeof(ReadWriteLock_VTable));
    readwritelock_vtable->read_lock = readwritelock_read_lock;
    readwritelock_vtable->read_unlock = readwritelock_read_unlock;
    readwritelock_vtable->write_lock = readwritelock_write_lock;
    readwritelock_vtable->write_unlock = readwritelock_write_unlock;
}
ReadWriteLock* readwritelock_new() {
    struct ReadWriteLock_* readwritelock_ = heap_alloc(sizeof(struct ReadWriteLock_));

    // set vtable
    readwritelock_->self.vtable = readwritelock_vtable;

    // set constructor data

    // set private data
    readwritelock_->share = NULL;
    readwritelock_->critical_lock = NULL;
    readwritelock_->write_lock = NULL;

    return (ReadWriteLock*)readwritelock_;
}
void readwritelock_free(ReadWriteLock* readwritelock) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)readwritelock;

    // free private data
    if(readwritelock_->share != NULL){
        share_free(readwritelock_->share);
    }
    if(readwritelock_->critical_lock != NULL){
        mutexlock_free(readwritelock_->critical_lock);
    }
    if(readwritelock_->write_lock != NULL){
        mutexlock_free(readwritelock_->write_lock);
    }

    // free constructor data

    // free self
    heap_free(readwritelock_);
}
ReadWriteLock* readwritelock_new_anonymous(){
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)readwritelock_new();

    // set constructor data

    // set private data
    // open readers share
    readwritelock_->share = share_new_anonymous(sizeof(int), 0);

    // if connections is 1, init share
    if(readwritelock_->share->vtable->connections(readwritelock_->share) <= 1){
        // get readers count address
        int* readers = readwritelock_->share->vtable->address(readwritelock_->share);

        // init readers
        (*readers) = 0;
    }

    // open critical mutexlock
    readwritelock_->critical_lock = mutexlock_new_anonymous();

    // open write mutexlock
    readwritelock_->write_lock = mutexlock_new_anonymous();

    return (ReadWriteLock*)readwritelock_;
}
ReadWriteLock* readwritelock_new_named(char* name){
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)readwritelock_new();

    // set constructor data

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // open share readers share
    String* readwritelock_readers_name = string_new_printf("%s_rwl_r", name);
    readwritelock_->share = share_new_named(readwritelock_readers_name->vtable->value(readwritelock_readers_name), sizeof(int), 0);
    string_free(readwritelock_readers_name);

    // if share connections is 1, init share
    if(readwritelock_->share->vtable->connections(readwritelock_->share) <= 1){
        // get readers count address
        int* readers = readwritelock_->share->vtable->address(readwritelock_->share);

        // init readers
        (*readers) = 0;
    }

    // open share critical mutexlock
    String* readwritelock_critical_name = string_new_printf("%s_rwl_c", name);
    readwritelock_->critical_lock = mutexlock_new_named(readwritelock_critical_name->vtable->value(readwritelock_critical_name));
    string_free(readwritelock_critical_name);

    // open share write mutexlock
    String* readwritelock_write_name = string_new_printf("%s_rwl_w", name);
    readwritelock_->write_lock = mutexlock_new_named(readwritelock_write_name->vtable->value(readwritelock_write_name));
    string_free(readwritelock_write_name);

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (ReadWriteLock*)readwritelock_;
}

#endif
