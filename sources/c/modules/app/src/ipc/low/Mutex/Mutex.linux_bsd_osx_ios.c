#include <ipc/low/Mutex.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <dsa/low/String.h>
#include <fcntl.h>
#include <local/low/Time.h>
#include <memory/low/Heap.h>
#include <pthread.h>
#include <sys/mman.h>

struct Mutex_ {
    Mutex self;
    void* memory;
    String* name;
};

// link methods
int mutex_acquire(struct Mutex* self, uint_64 timeout);
int mutex_release(struct Mutex* self);

// local methods
void* mutex_anonymous_new(int mode);
void mutex_anonymous_free(void* memory);
void* mutex_named_new(int mode, char* name);
void mutex_named_free(void* memory, char* name);

void* mutex_anonymous_new(int mode) {
    // alocate mutex
    void* result = heap_alloc(sizeof(pthread_mutex_t));

    // get mutex address
    pthread_mutex_t* mutex = result;

    // init mutex
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 0);

    // set mutex type
    switch (mode) {
        case 0:
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
            break;
        case 1:
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
            break;
        case 2:
            pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_ERRORCHECK);
            break;
    }

    pthread_mutex_init(mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);

    return result;
}
void mutex_anonymous_free(void* memory) {
    // get mutex address
    pthread_mutex_t* mutex = memory;

    // destroy internal mutex
    pthread_mutex_destroy(mutex);

    heap_free(memory);
}
void* mutex_named_new(int mode, char* name) {
    // check share memory exists
    bool exists = true;
    int exists_fd = shm_open(name, O_CREAT | O_EXCL, 0660);
    if (exists_fd > 0) {
        // not exists, create it
        close(exists_fd);
        exists = false;
    }

    // alocate share mutex and connections
    int fd = shm_open(name, O_CREAT | O_RDWR, 0660);
    void* result = mmap(NULL, sizeof(pthread_mutex_t) + sizeof(int), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    close(fd);

    // check error
    if (result == NULL || result == MAP_FAILED) {
        return NULL;
    }

    // get mutex and connections address
    pthread_mutex_t* mutex = result;
    int* connections = result + sizeof(pthread_mutex_t);

    // create and init mutex or open and increase connections
    if (!exists) {
        *connections = 1;

        // init share mutex
        pthread_mutexattr_t mattr;
        pthread_mutexattr_init(&mattr);
        pthread_mutexattr_setpshared(&mattr, 1);

        // set mutex type
        switch (mode) {
            case 0:
                pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_NORMAL);
                break;
            case 1:
                pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
                break;
            case 2:
                pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_ERRORCHECK);
                break;
        }

        pthread_mutex_init(mutex, &mattr);
        pthread_mutexattr_destroy(&mattr);
    } else {
        *connections += 1;
    }

    return result;
}
void mutex_named_free(void* memory, char* name) {
    // get mutex and connections address
    pthread_mutex_t* mutex = memory;
    int* connections = memory + sizeof(pthread_mutex_t);

    // destroy mutex and share memory on close all connections
    if (connections <= 1) {
        pthread_mutex_destroy(mutex);
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(int));
        shm_unlink(name);
    } else {
        *connections -= 1;
        munmap(memory, sizeof(pthread_mutex_t) + sizeof(int));
    }
}

// implement methods
int mutex_acquire(struct Mutex* self, uint_64 timeout) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // get mutex address
    pthread_mutex_t* mutex = mutex_->memory;

    // aquire the pthread mutex
    int result = -1;
    if (timeout == 0) {
        // try
        if (pthread_mutex_trylock(mutex) == 0) {
            result = 0;
        }
    } else if (timeout == UINT_64_MAX) {
        // infinity
        if (pthread_mutex_lock(mutex) == 0) {
            result = 0;
        }
    } else {
        // timed
        uint_64 time = time_epochmillis();

        // try lock until timeout
        while ((time_epochmillis() - time) <= timeout) {
            // try
            if (pthread_mutex_trylock(mutex) == 0) {
                result = 0;
                break;
            }
        }
    }

    return result;
}
int mutex_release(struct Mutex* self) {
    struct Mutex_* mutex_ = (struct Mutex_*)self;

    // get mutex address
    pthread_mutex_t* mutex = mutex_->memory;

    // release the pthread mutex
    int result = -1;
    if (pthread_mutex_unlock(mutex) == 0) {
        result = 0;
    }

    return result;
}

Mutex* mutex_new(int mode, char* name) {
    struct Mutex_* mutex_ = heap_alloc(sizeof(struct Mutex_));

    // init private methods
    mutex_->self.acquire = mutex_acquire;
    mutex_->self.release = mutex_release;

    if (name == NULL) {
        mutex_->name = NULL;

        // create internal mutex
        mutex_->memory = mutex_anonymous_new(mode);
    } else {
        mutex_->name = string_new_concat(name, "/mutex");

        // try acquire critical mutex
        if (critical != NULL) {
            critical->acquire(critical, UINT_64_MAX);
        }

        // create and init or open internal share mutex
        mutex_->memory = mutex_named_new(mode, mutex_->name->value(mutex_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->release(critical);
        }
    }

    return (Mutex*)mutex_;
}
void mutex_free(Mutex* mutex) {
    struct Mutex_* mutex_ = (struct Mutex_*)mutex;

    if (mutex_->name == NULL) {
        // destroy internal mutex
        mutex_anonymous_free(mutex_->memory);
    } else {
        // try acquire critical mutex
        if (critical != NULL) {
            critical->acquire(critical, UINT_64_MAX);
        }

        // destroy and close or close internal share mutex
        mutex_named_free(mutex_->memory, mutex_->name->value(mutex_->name));

        // try release critical mutex
        if (critical != NULL) {
            critical->release(critical);
        }

        string_free(mutex_->name);
    }

    heap_free(mutex_);
}

#endif