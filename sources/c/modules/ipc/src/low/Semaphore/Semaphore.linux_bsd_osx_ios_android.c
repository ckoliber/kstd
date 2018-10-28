#include <low/Semaphore.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>

struct Semaphore_ {
    // self public object
    Semaphore self;

    // constructor data

    // private data
    Share* share;
};

struct Semaphore_Memory{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int value;
};

// vtable
Semaphore_VTable* semaphore_vtable;

// link methods
int semaphore_wait(Semaphore* self, uint_64 timeout);
int semaphore_post(Semaphore* self);
int semaphore_get(Semaphore* self);

// implement methods
// vtable operators
int semaphore_wait(Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get memory address
    struct Semaphore_Memory* memory = semaphore_->share->vtable->address(semaphore_->share);

    // wait the value
    int result = 0;
    if(pthread_mutex_lock(&(memory->mutex)) == 0){
        if(timeout == UINT_64_MAX){
            // infinity

            // wait
            while(memory->value == 0){
                if(pthread_cond_wait(&(memory->cond), &(memory->mutex)) != 0){
                    result = -1;
                    break;
                }
            }

            // check wait was success, decrease semaphore value
            if(result == 0){
                memory->value--;
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

            // timed wait
            while(memory->value == 0){
                if(pthread_cond_timedwait(&(memory->cond), &(memory->mutex), &time_out) != 0){
                    result = -1;
                    break;
                }
            }

            // check wait was success, decrease semaphore value
            if(result == 0){
                memory->value--;
            }
        }

        // unlock mutex
        pthread_mutex_unlock(&(memory->mutex));
    }

    return result;
}
int semaphore_post(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get memory address
    struct Semaphore_Memory* memory = semaphore_->share->vtable->address(semaphore_->share);

    // post the value
    int result = -1;
    if(pthread_mutex_lock(&(memory->mutex)) == 0) {
        // increase semaphore value
        memory->value++;

        // notify
        if (pthread_cond_signal(&(memory->cond)) == 0) {
            result = 0;
        }

        // unlock mutex
        pthread_mutex_unlock(&(memory->mutex));
    }

    return result;
}
int semaphore_get(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get memory address
    struct Semaphore_Memory* memory = semaphore_->share->vtable->address(semaphore_->share);

    // get the value
    int result = -1;
    if(pthread_mutex_lock(&(memory->mutex)) == 0) {
        // get semaphore value
        result = memory->value;

        // unlock mutex
        pthread_mutex_unlock(&(memory->mutex));
    }

    return result;
}

// object allocation and deallocation operators
void semaphore_init() {
    // init vtable
    semaphore_vtable = heap_alloc(sizeof(Semaphore_VTable));
    semaphore_vtable->wait = semaphore_wait;
    semaphore_vtable->post = semaphore_post;
    semaphore_vtable->get= semaphore_get;
}
Semaphore* semaphore_new() {
    struct Semaphore_* semaphore_ = heap_alloc(sizeof(struct Semaphore_));

    // set vtable
    semaphore_->self.vtable = semaphore_vtable;

    // set constructor data

    // set private data
    semaphore_->share = NULL;

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    // free private data
    if (semaphore_->share != NULL) {
        // if share connections is 1, destroy
        if(semaphore_->share->vtable->connections(semaphore_->share) <= 1){
            struct Semaphore_Memory* memory = semaphore_->share->vtable->address(semaphore_->share);

            // destroy
            pthread_mutex_destroy(&(memory->mutex));
            pthread_cond_destroy(&(memory->cond));
        }

        share_free(semaphore_->share);
    }

    // free constructor data

    // free self
    heap_free(semaphore_);
}
Semaphore* semaphore_new_anonymous(int value){
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data

    // set private data
    // open errorcheck lock
    semaphore_->share = share_new_anonymous(sizeof(struct Semaphore_Memory), 0);

    // if share connections is 1, init share
    if(semaphore_->share->vtable->connections(semaphore_->share) <= 1){
        // get memory address
        struct Semaphore_Memory* memory = semaphore_->share->vtable->address(semaphore_->share);

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

        // init value
        memory->value = value;
    }

    return (Semaphore*)semaphore_;
}
Semaphore* semaphore_new_named(char* name, int value){
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // open share errorcheck lock
    String* semaphore_name = string_new_printf("%s_sm", name);
    semaphore_->share = share_new_named(semaphore_name->vtable->value(semaphore_name), sizeof(struct Semaphore_Memory), 0);
    string_free(semaphore_name);

    // if share connections is 1, init share
    if(semaphore_->share->vtable->connections(semaphore_->share) <= 1){
        // get memory address
        struct Semaphore_Memory* memory = semaphore_->share->vtable->address(semaphore_->share);

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

        // init value
        memory->value = value;
    }

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Semaphore*)semaphore_;
}

#endif
