#include <memory/low/Type.h>

#define FILE_ACCESS_READ   // file read access
#define FILE_ACCESS_WRITE  // file write access

#define FILE_LOCK_READ   // mandatory read lock
#define FILE_LOCK_WRITE  // mandatory write lock

#define FILE_MODE_OPEN       // exists => open, not exists => create and open
#define FILE_MODE_CREATE     // exists => truncate, not exists => create and open
#define FILE_MODE_TRYOPEN    // exists => open, not exists => error
#define FILE_MODE_TRYCREATE  // exists => error, not exists => create and open

#define FILE_FLAG_APPEND
#define FILE_FLAG_SYNC
#define FILE_FLAG_ASYNC
#define FILE_FLAG_NONBLOCK
#define FILE_FLAG_NOBUFFER

#define FILE_FLAG_NOBUFFER
#define FILE_FLAG_NOBUFFER
#define FILE_FLAG_NOBUFFER
#define FILE_FLAG_NOBUFFER

// vtable
typedef struct File_VTable {
    // io operators
    tsize (*read)(struct File* self, void* data, tsize size);
    tsize (*write)(struct File* self, void* data, tsize size);
    int (*flush)(struct File* self);
    int (*cancel)(struct File* self);

    // fd operators
    int (*reopen)(struct File* self, int access, int lock, int mode, int flags);
    int (*fd)(struct File* self);
    char* (*uri)(struct File* self);

    // cursor operators
    tsize (*seek)(struct File* self, tsize from, tsize position);
    tsize (*cursor)(struct File* self);
    tsize (*size)(struct File* self);

    // advisory lock operators
    // int (*lock)(struct File* self, int lock);
    // int (*unlock)(struct File* self, int lock);
} File_VTable;

// vtable + private data problem solve
typedef struct File {
    File_VTable* vtable;
} File;

// init vtable
void file_init();

// new raw file
File* file_new();

// free raw file
void file_free(File* file);

// new file
File* file_new_open(char* uri, int access, int lock, int mode, int flags);
File* file_new_duplicate(File* file);
File* file_new_accept(File* file);

// file move, copy, remove, link
int file_move(char* from_uri, char* to_uri, int mode);

// file://{path}
// dir://{path}
// std://input
// std://output
// std://error
// pipe://{name}
// socket://{host}:{port}
// socket://{backlog}:{host}:{port}
