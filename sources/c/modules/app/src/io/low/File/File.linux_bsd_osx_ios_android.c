#include <io/low/File.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <dsa/low/String.h>
#include <memory/low/Heap.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>

struct File_ {
    // self public object
    File self;

    // constructor data

    // private data
    int fd1;
    int fd2;
    String* uri;
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
tsize file_seek(struct File* self, tsize from, int position);

// implement methods
// vtable operators
tsize file_read(struct File* self, void* data, tsize size) {
    struct File_* file_ = (struct File_*)self;

    // read data
    tsize result = read(file_->fd1, data, size);

    return result;
}
tsize file_write(struct File* self, void* data, tsize size) {
    struct File_* file_ = (struct File_*)self;

    // write data
    tsize result = write(file_->fd1, data, size);

    return result;
}
int file_flush(struct File* self) {
    struct File_* file_ = (struct File_*)self;

    // flush data
    int result = fsync(file_->fd1);

    return result;
}
int file_cancel(struct File* self) {
    struct File_* file_ = (struct File_*)self;

    // cancel io
    int result = -1;
    int tempfd = dup(file_->fd1);
    if (close(file_->fd1) == 0) {
        result = dup2(tempfd, file_->fd1);
    }

    return result;
}
int file_reopen(struct File* self, int access, int lock, int mode, int flags) {
    struct File_* file_ = (struct File_*)self;

    // reopen fd
    int result = fcntl(file_->fd1, 1);

    return result;
}
int file_fd(struct File* self) {
    struct File_* file_ = (struct File_*)self;

    // get fd
    int result = file_->fd1;

    return result;
}
char* file_uri(struct File* self) {
    struct File_* file_ = (struct File_*)self;

    // get uri
    char* result = file_->uri->vtable->value(file_->uri);

    return result;
}
tsize file_seek(struct File* self, tsize from, int position) {
    struct File_* file_ = (struct File_*)self;

    // seek fd
    tsize result = lseek(file_->fd1, from, position);

    return result;
}

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
}
File* file_new() {
    struct File_* file_ = heap_alloc(sizeof(struct File_));

    // set vtable
    file_->self.vtable = file_vtable;

    // set constructor data

    // set private data
    file_->fd1 = -1;
    file_->fd2 = -1;
    file_->uri = NULL;

    return (File*)file_;
}
void file_free(File* file) {
    struct File_* file_ = (struct File_*)file;

    // free private data
    if (file_->fd1 > 0) {
        close(file_->fd1);
    }
    if (file_->fd2 > 0) {
        close(file_->fd2);
    }
    if (file_->uri != NULL) {
        string_free(file_->uri);
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
            file_->fd1 = open("", 1);
            // or for write or both
            break;
        case 2:
            // folder
            file_->fd1 = open("", 1);
            // or for write or both
            break;
        case 3:
            // stdin
            file_->fd1 = 0;
            break;
        case 4:
            // stdout
            file_->fd1 = 1;
            break;
        case 5:
            // stderr
            file_->fd1 = 2;
            break;
        case 6:
            // anonymous pipe
            int fds[2];
            if (pipe(fds) == 0) {
                file_->fd1 = fds[0];
                file_->fd2 = fds[1];
            }
            break;
        case 7:
            // named pipe
            if (mkfifo("", 0660) == 0) {
                file_->fd1 = open("", O_RDONLY);
            }
            break;
        case 8:
            // server socket
            file_->fd1 = socket(1, 1, 1);
            if (file_->fd1 != 0) {
            }
            if (bind(file_->fd1, NULL, 0) != 0) {
            }
            if (listen(file_->fd1, 1) != 0) {
            }
            break;
        case 9:
            // client socket
            file_->fd1 = socket(1, 1, 1);
            if (file_->fd1 != 0) {
            }
            if (connect(file_->fd1, NULL, 0) != 0) {
            }
            break;
    }
    file_->uri = string_new_copy(uri);

    return (File*)file_;
}
File* file_new_duplicate(File* file) {
    struct File_* file_ = (struct File_*)file_new();

    struct File_* file_from = (struct File_*)file;

    // set constructor data

    // set private data
    if (file_from->fd1 >= 0) {
        file_->fd1 = dup(file_from->fd1);
    }
    if (file_from->fd2 >= 0) {
        file_->fd2 = dup(file_from->fd2);
    }
    if (file_from->uri != NULL) {
        file_->uri = string_new_copy(file_from->uri->vtable->value(file_from->uri));
    }

    return (File*)file_;
}
File* file_new_accept(File* file) {
    struct File_* file_ = (struct File_*)file_new();

    struct File_* file_from = (struct File_*)file;

    // set constructor data

    // set private data
    if (file_from->fd1 >= 0) {
        struct sockaddr_in address;
        socklen_t size = sizeof(address);
        file_->fd1 = accept(file_from->fd1, &address, &size);
        // file_->uri = string_new_copy("...");
    }
    if (file_from->fd2 >= 0) {
        struct sockaddr_in address;
        socklen_t size = sizeof(address);
        file_->fd2 = accept(file_from->fd2, &address, &size);
        // file_->uri = string_new_copy("...");
    }

    return (File*)file_;
}

// file move, copy, remove, link
int file_move(char* from_uri, char* to_uri, int mode) {
}

#endif