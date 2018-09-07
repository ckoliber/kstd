#include <memory/low/Share.h>

#if defined(APP_WINDOWS)

#include <memory/low/Heap.h>

struct Share_ {
    struct Share self;
    String* name;
    Void* address;
    HANDLE handle;
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
    FlushViewOfFile(share_->address, size);

    return result;
}

Share* share_new(Char* name, Size offset, Size size) {
    struct Share_* share_ = heap_alloc(sizeof(struct Share_));

    // init private methods
    share_->self.address = share_address;
    share_->self.flush = share_flush;

    // create name and fd and map shm
    share_->name = string_new_copy(name);
    share_->handle = CreateFileMappingW(
        INVALID_HANDLE_VALUE,
        NULL,
        PAGE_EXECUTE_READWRITE,
        0,
        0,
        share_->name->value(share_->name));
    share_->address = MapViewOfFile(share->handle, FILE_MAP_ALL_ACCESS, 0, 0, size);

    // check error
    if (share_->handle == INVALID_HANDLE_VALUE || share_->address == NULL) {
        share_free((Share*)share_);
        return NULL;
    }

    return (Share*)share_;
}

Void share_free(Share* share) {
    struct Share_* share_ = (struct Share_*)share;

    // unmap shm and close handle
    UnmapViewOfFile(share_->address);
    CloseHandle(share_->handle);

    // destroy name and fd and unmap shm
    string_free(share_->name);

    heap_free(share_);
}

#endif