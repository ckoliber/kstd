#include <memory/low/Share.h>

#if defined(APP_LINUX)

#include <fcntl.h>
#include <memory/low/Heap.h>
#include <pthread.h>
#include <sys/mman.h>

struct Share_ {
    struct Share self;
    String* name;
    int fd;
    void* address;
    tsize size;
};

// link methods
void* share_address(struct Share* self);
void share_flush(struct Share* self, tsize size);

void share_create(struct Share_* share_);
void share_increase(struct Share_* share_);
void share_decrease(struct Share_* share_);
void share_destroy(struct Share_* share_);

// implement methods
void share_create(struct Share_* share_) {
    // init shm mutex
    pthread_mutex_t mutex;
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 1);
    pthread_mutex_init(&mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);
    heap_copy(share_->address, &mutex, sizeof(pthread_mutex_t));

    // init shm int
    int connections = 0;
    heap_copy(share_->address + sizeof(pthread_mutex_t), &connections, sizeof(int));
}
void share_increase(struct Share_* share_) {
    pthread_mutex_t* mutex = share_->address;

    // begin critical section
    pthread_mutex_lock(mutex);

    // increase connections number
    int* connections = share_->address + sizeof(pthread_mutex_t);
    *connections++;

    // end critical section
    pthread_mutex_unlock(mutex);
}
void share_decrease(struct Share_* share_) {
    pthread_mutex_t* mutex = share_->address;

    // begin critical section
    pthread_mutex_lock(mutex);

    // increase connections number
    int* connections = share_->address + sizeof(pthread_mutex_t);
    *connections--;

    // end critical section
    pthread_mutex_unlock(mutex);
}
void share_destroy(struct Share_* share_) {
    pthread_mutex_t* mutex = share_->address;

    // begin critical section
    pthread_mutex_lock(mutex);

    // check destroy
    int* connections = share_->address + sizeof(pthread_mutex_t);
    bool destroy = (*connections == 0);

    // end critical section
    pthread_mutex_unlock(mutex);

    if (destroy) {
        pthread_mutex_destroy(mutex);
    }
}

void* share_address(struct Share* self) {
    struct Share_* share_ = (struct Share_*)self;

    // get mapped shm address
    void* result = share_->address + sizeof(pthread_mutex_t);

    return result;
}
void share_flush(struct Share* self, tsize size) {
    struct Share_* share_ = (struct Share_*)self;

    // flush mapped shm to disk
    msync(share_->address, sizeof(pthread_mutex_t) + size, MS_SYNC);
}

Share* share_new(char* name, tsize size) {
    struct Share_* share_ = heap_alloc(sizeof(struct Share_));

    // init private methods
    share_->self.address = share_address;
    share_->self.flush = share_flush;

    // check exists
    bool exists = true;
    int exists_fd = shm_open(name, O_CREAT | O_EXCL, 0660);
    if (exists_fd > 0) {
        exists = false;
        close(exists_fd);
    }

    // create name and fd and map shm
    share_->name = string_new_copy(name);
    share_->fd = shm_open(share_->name->value(share_->name), O_CREAT | O_RDWR, 0660);
    share_->address = mmap(NULL, sizeof(pthread_mutex_t) + sizeof(int) + size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, share_->fd, 0);
    share_->size = size;

    // check error
    if (share_->fd <= 0 || share_->address == NULL || share_->address == MAP_FAILED) {
        share_free((Share*)share_);
        return NULL;
    }

    // create and connections increase
    if (!exists) {
        share_create(share_);
    }
    share_increase(share_);

    return (Share*)share_;
}
void share_free(Share* share) {
    struct Share_* share_ = (struct Share_*)share;

    // connections decrease and destroy
    share_decrease(share_);
    share_destroy(share_);

    // unmap shm and close handle
    munmap(share_->address, share_->size);
    close(share_->fd);

    // destroy name and fd and unmap shm
    string_free(share_->name);

    heap_free(share_);
}

#endif