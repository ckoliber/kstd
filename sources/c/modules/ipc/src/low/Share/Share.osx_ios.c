#include <low/Share.h>

#if defined(APP_OSX) || defined(APP_IOS)

#include <low/ReentrantLock.h>
#include <low/Heap.h>
#include <low/String.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

struct Share_ {
    // self public object
    Share self;

    // constructor data
    String* name;
    tsize size;
    tsize offset;

    // private data
    void* memory;
};

// vtable
Share_VTable* share_vtable;

// link methods
void* share_address(Share* self);
int share_connections(Share* self);
int share_flush(Share* self, tsize size);

// local methods
void* share_named_new(char* name, tsize size, tsize offset);
void share_named_free(void* memory, char* name, tsize size, tsize offset);
void* share_anonymous_new(tsize size, tsize offset);
void share_anonymous_free(void* memory);

// implement methods
void* share_named_new(char* name, tsize size, tsize offset){
    // check share memory exists
    bool exists = true;
    int exists_fd = shm_open(name, O_CREAT | O_EXCL, 0660);
    if (exists_fd > 0) {
        // not exists, it was created now
        close(exists_fd);
        exists = false;
    }

    // allocate share
    int fd = shm_open(name, O_CREAT | O_RDWR, 0660);
    if(!exists){
        ftruncate(fd, sizeof(int) + size);
    }
    void* result = mmap(NULL, sizeof(int) + size, PROT_READ | PROT_WRITE , MAP_SHARED, fd, 0);
    close(fd);

    // check error
    if (result == NULL || result == MAP_FAILED) {
        return NULL;
    }

    // get connections address
    int* connections = result;

    // init connections
    if (!exists) {
        *connections = 1;
    } else {
        (*connections)++;
    }

    return result;
}
void share_named_free(void* memory, char* name, tsize size, tsize offset){
    // get connections address
    int* connections = memory;

    if (*connections == 1) {
        // unmap share memory
        munmap(memory, sizeof(int) + size);

        // unlink (it has not any connections)
        shm_unlink(name);
    } else {
        // decrease connections
        (*connections)--;

        // unmap share memory
        munmap(memory, sizeof(int) + size);

        // dont unlink (it has another connections)
    }
}
void* share_anonymous_new(tsize size, tsize offset){
    void* result = heap_alloc(sizeof(int) + size);

    // get connections address
    int* connections = result;

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
    void* result = (char*) share_->memory + sizeof(int);

    return result;
}
int share_connections(Share* self){
    struct Share_* share_ = (struct Share_*)self;

    // get connections address
    int result = *(int*) share_->memory;

    return result;
}
int share_flush(Share* self, tsize size){
    struct Share_* share_ = (struct Share_*)self;

    // flush memory
    if(msync(share_->memory, size, MS_SYNC) == 0){
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
            share_named_free(share_->memory, share_->name->vtable->value(share_->name), share_->size, share_->offset);

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
    share_->memory = share_anonymous_new(share_->size, share_->offset);

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
    share_->memory = share_named_new(share_->name->vtable->value(share_->name), share_->size, share_->offset);

    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Share*)share_;
}

#endif
