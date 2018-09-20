// #include <memory/low/Share.h>

// #if defined(APP_WINDOWS)

// #include <dsa/low/String.h>
// #include <memory/low/Heap.h>

// struct Share_ {
//     struct Share self;
//     String* name;
//     void* address;
//     HANDLE handle;
// };

// // link methods
// void* share_address(struct Share* self);
// void share_flush(struct Share* self, tsize size);
// int share_connections(struct Share* self);

// // implement methods
// void* share_address(struct Share* self) {
//     struct Share_* share_ = (struct Share_*)self;

//     // get mapped shm address
//     void* result = share_->address;

//     return result;
// }
// void share_flush(struct Share* self, tsize size) {
//     struct Share_* share_ = (struct Share_*)self;

//     // flush mapped shm to disk
//     FlushViewOfFile(share_->address, size);
// }
// int share_connections(struct Share* self) {
//     struct Share_* share_ = (struct Share_*)self;

//     // get connections
//     int result = -1;

//     return result;
// }

// Share* share_new(char* name, tsize size) {
//     struct Share_* share_ = heap_alloc(sizeof(struct Share_));

//     // init private methods
//     share_->self.address = share_address;
//     share_->self.flush = share_flush;
//     share_->self.connections = share_connections;

//     // create name and fd and map shm
//     share_->name = string_new_copy(name);
//     share_->handle = CreateFileMappingW(
//         INVALID_HANDLE_VALUE,
//         NULL,
//         PAGE_EXECUTE_READWRITE,
//         0,
//         0,
//         share_->name->value(share_->name));
//     share_->address = MapViewOfFile(share->handle, FILE_MAP_ALL_ACCESS, 0, 0, size);

//     // check error
//     if (share_->handle == INVALID_HANDLE_VALUE || share_->address == NULL) {
//         share_free((Share*)share_);
//         return NULL;
//     }

//     return (Share*)share_;
// }
// void share_free(Share* share) {
//     struct Share_* share_ = (struct Share_*)share;

//     // unmap shm and close handle
//     UnmapViewOfFile(share_->address);
//     CloseHandle(share_->handle);

//     // destroy name and fd and unmap shm
//     string_free(share_->name);

//     heap_free(share_);
// }

// #endif