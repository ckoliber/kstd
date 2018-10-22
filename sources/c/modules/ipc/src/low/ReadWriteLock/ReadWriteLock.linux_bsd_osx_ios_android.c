#include <low/ReadWriteLock.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>

struct ReadWriteLock_ {
    // self public object
    ReadWriteLock self;

    // constructor data

    // private data
    Share* share;
};

struct ReadWriteLock_Memory{
    pthread_mutex_t critical;
    pthread_mutex_t write;
    int readers;
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

    // get memory address
    struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

    // lock the pthread write mutex
    int result = -1;
    if (timeout == UINT_64_MAX) {
        // infinity

        // begin critical
        if (pthread_mutex_lock(&(memory->critical)) == 0) {

            // check readers count
            if(memory->readers == 0){
                // lock the write mutex
                if(pthread_mutex_lock(&(memory->write)) == 0){
                    memory->readers++;
                    result = 0;
                }
            }else{
                // write mutex was locked before
                memory->readers++;
                result = 0;
            }

            // end critical
            pthread_mutex_unlock(&(memory->critical));
        }
    } else if(timeout > 0){
        // timed

        // begin critical
        if (pthread_mutex_lock(&(memory->critical)) == 0) {

            // check readers count
            if(memory->readers == 0){
                // timed lock the write mutex

                // get time_out
                struct timeval time_now;
                struct timespec time_out;
                gettimeofday(&time_now, NULL);
                time_out.tv_sec = time_now.tv_sec;
                time_out.tv_nsec = time_now.tv_usec * 1000;
                time_out.tv_sec += timeout / 1000;
                time_out.tv_nsec += (timeout % 1000) * 1000000;

                // timed lock
                if (pthread_mutex_timedlock(&(memory->write), &time_out) == 0) {
                    memory->readers++;
                    result = 0;
                }
            }else{
                // write mutex was locked before
                memory->readers++;
                result = 0;
            }

            // end critical
            pthread_mutex_unlock(&(memory->critical));
        }
    }else{
        // try

        // begin critical
        if (pthread_mutex_lock(&(memory->critical)) == 0) {

            // check readers count
            if(memory->readers == 0){
                // try lock the write mutex
                if(pthread_mutex_trylock(&(memory->write)) == 0){
                    memory->readers++;
                    result = 0;
                }
            }else{
                // write mutex was locked before
                memory->readers++;
                result = 0;
            }

            // end critical
            pthread_mutex_unlock(&(memory->critical));
        }
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

    // get memory address
    struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

    // unlock the pthread write mutex
    int result = -1;

    // begin critical
    if (pthread_mutex_lock(&(memory->critical)) == 0) {

        // check readers count
        if(memory->readers == 1){
            // lock the write mutex
            if(pthread_mutex_unlock(&(memory->write)) == 0){
                memory->readers--;
                result = 0;
            }
        }else{
            // write mutex was locked before
            memory->readers--;
            result = 0;
        }

        // end critical
        pthread_mutex_unlock(&(memory->critical));
    }

    return result;
}
int readwritelock_write_lock(ReadWriteLock* self, uint_64 timeout) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)self;

    // get memory address
    struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

    // lock the pthread write mutex
    if (timeout == UINT_64_MAX) {
        // infinity

        // lock the write mutex
        if(pthread_mutex_lock(&(memory->write)) == 0){
            return 0;
        }
    } else if(timeout > 0){
        // timed

        // timed lock the write mutex

        // get time_out
        struct timeval time_now;
        struct timespec time_out;
        gettimeofday(&time_now, NULL);
        time_out.tv_sec = time_now.tv_sec;
        time_out.tv_nsec = time_now.tv_usec * 1000;
        time_out.tv_sec += timeout / 1000;
        time_out.tv_nsec += (timeout % 1000) * 1000000;

        // timed lock
        if (pthread_mutex_timedlock(&(memory->write), &time_out) == 0) {
            return 0;
        }
    }else{
        // try

        // try lock the write mutex
        if(pthread_mutex_trylock(&(memory->write)) == 0){
            return 0;
        }
    }

    return -1;
}
int readwritelock_write_unlock(ReadWriteLock* self) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)self;

    // get memory address
    struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

    // unlock the pthread write mutex
    if(pthread_mutex_unlock(&(memory->write)) == 0){
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
    readwritelock_->share = NULL;

    return (ReadWriteLock*)readwritelock_;
}
void readwritelock_free(ReadWriteLock* readwritelock) {
    struct ReadWriteLock_* readwritelock_ = (struct ReadWriteLock_*)readwritelock;

    // free private data
    if (readwritelock_->share != NULL) {
        // if share connections is 1, destroy
        if(readwritelock_->share->vtable->connections(readwritelock_->share) <= 1){
            struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

            // destroy
            pthread_mutex_destroy(&(memory->critical));
            pthread_mutex_destroy(&(memory->write));
        }

        share_free(readwritelock_->share);
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

        // open share errorcheck lock
        String* readwritelock_name = string_new_printf("%s_readwritelock", name);
        readwritelock_->share = share_new_object(readwritelock_name->vtable->value(readwritelock_name), sizeof(struct ReadWriteLock_Memory), 0);
        string_free(readwritelock_name);

        // if share connections is 1, init share
        if(readwritelock_->share->vtable->connections(readwritelock_->share) <= 1){
            // get memory address
            struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

            // init critical mutex
            pthread_mutexattr_t critical_mattr;
            pthread_mutexattr_init(&critical_mattr);
            pthread_mutexattr_setpshared(&critical_mattr, PTHREAD_PROCESS_SHARED);
            pthread_mutexattr_settype(&critical_mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->critical), &critical_mattr);
            pthread_mutexattr_destroy(&critical_mattr);

            // init write mutex
            pthread_mutexattr_t write_mattr;
            pthread_mutexattr_init(&write_mattr);
            pthread_mutexattr_setpshared(&write_mattr, PTHREAD_PROCESS_SHARED);
            pthread_mutexattr_settype(&write_mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->critical), &write_mattr);
            pthread_mutexattr_destroy(&write_mattr);

            // init readers count
            memory->readers = 0;
        }

        // try unlock critical
        if (critical != NULL) {
            critical->vtable->unlock(critical);
        }
    } else {
        // open errorcheck lock
        readwritelock_->share = share_new_object(NULL, sizeof(struct ReadWriteLock_Memory), 0);

        // if share connections is 1, init share
        if(readwritelock_->share->vtable->connections(readwritelock_->share) <= 1){
            // get memory address
            struct ReadWriteLock_Memory* memory = (struct ReadWriteLock_Memory*) readwritelock_->share->vtable->address(readwritelock_->share);

            // init critical mutex
            pthread_mutexattr_t critical_mattr;
            pthread_mutexattr_init(&critical_mattr);
            pthread_mutexattr_setpshared(&critical_mattr, PTHREAD_PROCESS_PRIVATE);
            pthread_mutexattr_settype(&critical_mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->critical), &critical_mattr);
            pthread_mutexattr_destroy(&critical_mattr);

            // init write mutex
            pthread_mutexattr_t write_mattr;
            pthread_mutexattr_init(&write_mattr);
            pthread_mutexattr_setpshared(&write_mattr, PTHREAD_PROCESS_PRIVATE);
            pthread_mutexattr_settype(&write_mattr, PTHREAD_MUTEX_NORMAL);
            pthread_mutex_init(&(memory->write), &write_mattr);
            pthread_mutexattr_destroy(&write_mattr);

            // init readers count
            memory->readers = 0;
        }
    }

    return (ReadWriteLock*)readwritelock_;
}

#endif
