#include <low/Share.h>

#if defined(APP_WINDOWS)

#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>

struct Share_ {
    // self public object
    Share self;

    // constructor data
    String* name;
    tsize size;
    tsize offset;

    // private data
    void* memory;
    HANDLE memory_handle;
    int connections;
};

// vtable
Share_VTable* share_vtable;

// link methods
void* share_address(Share* self);
int share_connections(Share* self);
int share_flush(Share* self, tsize size);

// local methods
void* share_named_new(char* name, HANDLE* memory_handle, tsize size, tsize offset, int* connections);
void share_named_free(void* memory, HANDLE memory_handle);
void* share_anonymous_new(tsize size, tsize offset, int* connections);
void share_anonymous_free(void* memory);

// implement methods
void* share_named_new(char* name, HANDLE* memory_handle, tsize size, tsize offset, int* connections){
    // check share memory exists
    bool exists = false;
    HANDLE exists_handle = OpenFileMappingA(0, FALSE, name);
    if (exists_handle != INVALID_HANDLE_VALUE) {
        // exists, it was created before
        CloseHandle(exists_handle);
        exists = true;
    }

    // allocate share start and end and queue
    HANDLE handle = CreateFileMappingA(
            INVALID_HANDLE_VALUE,
            NULL,
            PAGE_EXECUTE_READWRITE,
            0,
            size,
            name);
    void* result = MapViewOfFile(
            handle,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            size);

    // check error
    if (result == NULL) {
        CloseHandle(handle);
        return NULL;
    } else {
        *memory_handle = handle;
    }

    // init connections
    if (!exists) {
        *connections = 1;
    } else {
        *connections = 2;
    }

    return result;
}
void share_named_free(void* memory, HANDLE memory_handle){
    // unmap share memory
    UnmapViewOfFile(memory);

    // close share memory
    CloseHandle(memory_handle);
}
void* share_anonymous_new(tsize size, tsize offset, int* connections){
    void* result = heap_alloc(sizeof(int) + size);

    // init connections
    *connections = 1;

    return result;
}
void share_anonymous_free(void* memory){
    heap_free(memory);
}

// vtable operators
void* share_address(Share* self){
    struct Share_* share_ = (struct Share_*)self;

    // get memory address
    void* result = share_->memory;

    return result;
}
int share_connections(Share* self){
    struct Share_* share_ = (struct Share_*)self;

    // if shm was exists connections is 2, else is 1
    int result = share_->connections;

    return result;
}
int share_flush(Share* self, tsize size){
    struct Share_* share_ = (struct Share_*)self;

    // flush memory
    if(FlushViewOfFile(share_->memory, size) != 0){
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void share_init() {
    // init vtable
    share_vtable = heap_alloc(sizeof(Share_VTable));
    share_vtable->address = share_address;
    share_vtable->connections = share_connections;
    share_vtable->flush = share_flush;
}
Share* share_new() {
    struct Share_* share_ = heap_alloc(sizeof(struct Share_));

    // set vtable
    share_->self.vtable = share_vtable;

    // set constructor data
    share_->name = NULL;
    share_->size = 0;
    share_->offset = 0;

    // set private data
    share_->memory = NULL;
    share_->memory_handle = INVALID_HANDLE_VALUE;
    share_->connections = 0;

    return (Share*)share_;
}
void share_free(Share* share) {
    struct Share_* share_ = (struct Share_*)share;

    // free private data
    if(share_->memory != NULL){
        if(share_->name != NULL){
            // try lock critical
            if (critical != NULL) {
                critical->vtable->lock(critical, UINT_64_MAX);
            }

            // free named memory
            share_named_free(share_->memory, share_->memory_handle);

            // try unlock critical
            if (critical != NULL) {
                critical->vtable->unlock(critical);
            }
        }else{
            // free anonymous memory
            share_anonymous_free(share_->memory);
        }
    }

    // free constructor data
    if(share_->name != NULL){
        string_free(share_->name);
    }

    // free self
    heap_free(share_);
}
Share* share_new_anonymous(tsize size, tsize offset){
    struct Share_* share_ = (struct Share_*)share_new();

    // set constructor data
    share_->size = size;
    share_->offset = offset;

    // set private data
    // new anonymous memory
    share_->memory = share_anonymous_new(share_->size, share_->offset, &share_->connections);

    return (Share*)share_;
}
Share* share_new_named(char* name, tsize size, tsize offset){
    struct Share_* share_ = (struct Share_*)share_new();

    // set constructor data
    share_->name = string_new_printf("%s_sh", name);
    share_->size = size;
    share_->offset = offset;

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }

    // new named memory
    share_->memory = share_named_new(share_->name->vtable->value(share_->name), &(share_->memory_handle), share_->size, share_->offset, &share_->connections);

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Share*)share_;
}

#endif
