#include <io/low/File.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <memory/low/Heap.h>
#include <unistd.h>

#include <fcntl.h>

struct File_ {
    // self public object
    File self;

    // constructor data

    // private data
    int fd;
};

// vtable
File_VTable* file_vtable;

// link methods
tsize file_read(struct File* self, void* data, tsize size);
tsize file_write(struct File* self, void* data, tsize size);
int file_flush(struct File* self);
int file_cancel(struct File* self);
int file_reopen(struct File* self, int access, int lock, int mode, int flags);
int file_fd(struct File* self);
char* file_uri(struct File* self);
tsize file_seek(struct File* self, tsize from, tsize position);
tsize file_cursor(struct File* self);
tsize file_size(struct File* self);

// implement methods
// vtable operators
tsize file_read(struct File* self, void* data, tsize size) {}
tsize file_write(struct File* self, void* data, tsize size) {}
int file_flush(struct File* self) {}
int file_cancel(struct File* self) {}
int file_reopen(struct File* self, int access, int lock, int mode, int flags) {}
int file_fd(struct File* self) {}
char* file_uri(struct File* self) {}
tsize file_seek(struct File* self, tsize from, tsize position) {}
tsize file_cursor(struct File* self) {}
tsize file_size(struct File* self) {}

// object allocation and deallocation operators
void file_init() {
    // init vtable
    file_vtable = heap_alloc(sizeof(File_VTable));
    file_vtable->read = file_read;
    file_vtable->write = file_write;
    file_vtable->flush = file_flush;
    file_vtable->cancel = file_cancel;
    file_vtable->reopen = file_reopen;
    file_vtable->fd = file_fd;
    file_vtable->uri = file_uri;
    file_vtable->seek = file_seek;
    file_vtable->cursor = file_cursor;
    file_vtable->size = file_size;
}
File* file_new() {
    struct File_* file_ = heap_alloc(sizeof(struct File_));

    // set vtable
    file_->self.vtable = file_vtable;

    // set constructor data

    // set private data
    file_->fd = 0;

    return (File*)file_;
}
void file_free(File* file) {
    struct File_* file_ = (struct File_*)file;

    // free private data
    if (file_->fd > 0) {
        close(file_->fd);
    }

    // free self
    heap_free(file_);
}
File* file_new_open(char* uri, int access, int lock, int mode, int flags) {
    struct File_* file_ = (struct File_*)file_new();

    // set constructor data

    // set private data
    switch (uri[0]) {
        case 0:
            // file
            file_->fd = open("", );
            break;
        case 2:
            // folder
            break;
        case 3:
            // stdin
            break;
        case 4:
            // stdout
            break;
        case 5:
            // stderr
            break;
        case 6:
            // anonymous pipe
            break;
        case 7:
            // named pipe
            break;
        case 8:
            // server socket
            break;
        case 9:
            // client socket
            break;
    }

    return (File*)file_;
}
File* file_new_duplicate(File* file) {
    struct File_* file_ = (struct File_*)file_new();

    // set constructor data

    // set private data
    // file_->fd = open();

    return (File*)file_;
}
File* file_new_accept(File* file) {
    struct File_* file_ = (struct File_*)file_new();

    // set constructor data

    // set private data
    // file_->fd = open();

    return (File*)file_;
}

// file move, copy, remove, link
int file_move(char* from_uri, char* to_uri, int mode) {
}

#endif