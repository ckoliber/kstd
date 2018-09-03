#include <memory/low/Share.h>

#if defined(APP_LINUX)

#include <fcntl.h>
#include <memory/low/Heap.h>
#include <pthread.h>
#include <sys/mman.h>

struct Share_ {
    struct Share self;
    String* name;
    Void* address;
    SignedInt fd;
};

// link methods
Void* share_address(struct Share* self);
Void share_flush(struct Share* self);

// local methods

// implement methods
Void* share_address(struct Share* self) {
}
Void share_flush(struct Share* self) {
}

Share* share_new(Char* name, Size offset, Size size) {
    struct Share_* share_ = heap_alloc(sizeof(struct Share_));

    // init private methods
    share_->self.address = share_address;
    share_->self.flush = share_flush;

    // create name and fd and map shm
    share_->name = string_new(name);
    share_->fd = (SignedInt)shm_open(share_->name->value(share_->name), O_CREAT | O_RDWR, 0660);
    share_->address = mmap(NULL, size + sizeof(pthread_mutex_t) + sizeof(SignedInt), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, share_->fd, offset);

    // check error
    if (share_->fd <= 0 || share_->address == NULL) {
        share_free((Share*)share_);
        return NULL;
    }

    // init internal connections number
    SignedInt connections = 1;
    heap_copy(share_->address, &connections, sizeof(SignedInt));

    // init internal pthread_mutex_t
    pthread_mutex_t mutex;
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 1);
    pthread_mutex_init(&mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);

    return (Share*)share_;
}

Void share_free(Share* share) {
    struct Share_* share_ = (struct Share_*)share;

    // destroy name and fd and unmap shm
    string_free(share_->name);

    heap_free(share_);
}

#endif