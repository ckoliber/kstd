#include <io/low/File.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <memory/low/Heap.h>

struct File_ {
    File self;
    int fd;
};

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

// local methods
File* file_new();

// implement methods
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

File* file_new() {
    struct File_* file_ = heap_alloc(sizeof(struct File_));

    // init private methods
    file_->self.read = file_read;
    file_->self.write = file_write;
    file_->self.flush = file_flush;
    file_->self.cancel = file_cancel;
    file_->self.reopen = file_reopen;
    file_->self.fd = file_fd;
    file_->self.uri = file_uri;
    file_->self.seek = file_seek;
    file_->self.cursor = file_cursor;
    file_->self.size = file_size;

    return (File*)file_;
}
void file_free(File* file) {
}

File* file_new_open(char* uri, int access, int lock, int mode, int flags) {
    struct File_* file_ = (struct File_*)file_new();

    return (File*)file_;
}
File* file_new_duplicate(File* file) {}
File* file_new_accept(File* file) {}
int file_move(char* from_uri, char* to_uri, int mode) {}

#endif