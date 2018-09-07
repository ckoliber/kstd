#include <memory/low/Share.h>

#if defined(APP_ANDROID)

#include <ashmem.h>
#include <fcntl.h>
#include <memory/low/Heap.h>
#include <sys/mman.h>

struct Share_ {
    struct Share self;
    Size size;
    String* name;
    Void* address;
    int fd;
};

// link methods
Void* share_address(struct Share* self);
Void share_flush(struct Share* self, Size size);

// implement methods
Void* share_address(struct Share* self) {
    struct Share_* share_ = (struct Share_*)self;

    // get mapped shm address
    Void* result = share_->address;

    return result;
}
Void share_flush(struct Share* self, Size size) {
    struct Share_* share_ = (struct Share_*)self;

    // flush mapped shm to disk
    msync(share_->address, size, MS_SYNC);
}

Share* share_new(Char* name, Size size) {
    struct Share_* share_ = heap_alloc(sizeof(struct Share_));

    // init private methods
    share_->self.address = share_address;
    share_->self.flush = share_flush;

    // create name and fd and map shm
    share_->name = string_new_copy(name);
    share_->fd = ashmem_create_region(share_->name->value(share_->name), size);
    share_->address = mmap(NULL, size, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, share_->fd, 0);

    // check error
    if (share_->fd < 0 || share_->address == NULL || share_->address == MAP_FAILED) {
        share_free((Share*)share_);
        return NULL;
    }

    return (Share*)share_;
}
Void share_free(Share* share) {
    struct Share_* share_ = (struct Share_*)share;

    // unmap shm and close handle
    munmap(share_->address, share_->size);
    close(share_->fd);

    // destroy name and fd and unmap shm
    string_free(share_->name);

    heap_free(share_);
}

#endif