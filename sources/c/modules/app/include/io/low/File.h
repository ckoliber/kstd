#include <memory/low/Type.h>

typedef struct File {
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
