#include <low/ReadWriteLock.h>

#if defined(APP_WINDOWS)

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
    MutexLock* critical_lock;
    MutexLock* write_lock;
    Share* readers_share;
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
    int* readers = (int*) readwritelock_->readers_share->vtable->address(readwritelock_->readers_share);

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
    int* readers = (int*) readwritelock_->readers_share->vtable->address(readwritelock_->readers_share);

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
    readwritelock_vtable = (ReadWriteLock_VTable*) heap_alloc(sizeof(ReadWriteLock_VTable));
    readwritelock_vtable->read_lock = readwritelock_read_lock;
    readwritelock_vtable->read_unlock = readwritelock_read_unlock;
    readwritelock_vtable->write_lock = readwritelock_write_lock;
    readwritelock_vtable->write_unlock = readwritelock_write_unlock;
}
ReadWriteLock* readwritelock_new() {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*) heap_alloc(sizeof(struct ReadWriteLock_));

    // set vtable
    readwritelock_->self.vtable = readwritelock_vtable;

    // set constructor data

    // set private data
    readwritelock_->critical_lock = NULL;
    readwritelock_->write_lock = NULL;
    readwritelock_->readers_share = NULL;

    return (ReadWriteLock*)readwritelock_;
}
void readwritelock_free(ReadWriteLock* readwritelock) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)readwritelock;

    // free private data
    if(readwritelock_->critical_lock != NULL){
        mutexlock_free(readwritelock_->critical_lock);
    }
    if(readwritelock_->write_lock != NULL){
        mutexlock_free(readwritelock_->write_lock);
    }
    if(readwritelock_->readers_share != NULL){
        share_free(readwritelock_->readers_share);
    }

    // free constructor data

    // free self
    heap_free((uint_8*) readwritelock_);
}
ReadWriteLock* readwritelock_new_object(char* name) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)readwritelock_new();

    // set constructor data

    // set private data
    if (name != NULL) {
        // try lock critical
        if (critical != NULL) {
            critical->vtable->lock(critical, UINT_64_MAX);
        }

        // open share critical mutexlock
        String* readwritelock_critical_name = string_new_printf("%s_readwritelock_critical", name);
        readwritelock_->critical_lock = mutexlock_new_object(readwritelock_critical_name->vtable->value(readwritelock_critical_name));
        string_free(readwritelock_critical_name);

        // open share write mutexlock
        String* readwritelock_write_name = string_new_printf("%s_readwritelock_write", name);
        readwritelock_->write_lock = mutexlock_new_object(readwritelock_write_name->vtable->value(readwritelock_write_name));
        string_free(readwritelock_write_name);

        // open share readers share
        String* readwritelock_readers_name = string_new_printf("%s_readwritelock_readers", name);
        readwritelock_->readers_share = share_new_object(readwritelock_readers_name->vtable->value(readwritelock_readers_name), sizeof(int), 0);
        string_free(readwritelock_readers_name);

        // if share connections is 1, init share
        if(readwritelock_->readers_share->vtable->connections(readwritelock_->readers_share) <= 1){
            // get readers count address
            int* readers = (int*) readwritelock_->readers_share->vtable->address(readwritelock_->readers_share);

            // init readers
            (*readers) = 0;
        }

        // try unlock critical
        if (critical != NULL) {
            critical->vtable->unlock(critical);
        }
    } else {
        // open critical mutexlock
        readwritelock_->critical_lock = mutexlock_new_object(NULL);

        // open write mutexlock
        readwritelock_->write_lock = mutexlock_new_object(NULL);

        // open readers share
        readwritelock_->readers_share = share_new_object(NULL, sizeof(int), 0);

        // if connections is 1, init share
        if(readwritelock_->readers_share->vtable->connections(readwritelock_->readers_share) <= 1){
            // get readers count address
            int* readers = (int*) readwritelock_->readers_share->vtable->address(readwritelock_->readers_share);

            // init readers
            (*readers) = 0;
        }
    }

    return (ReadWriteLock*)readwritelock_;
}

#endif
