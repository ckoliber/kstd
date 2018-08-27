typedef struct File {
    int (*read)(struct File* self, void** data, int* size);
    int (*write)(struct File* self, void* data, int size);
    int (*flush)(struct File* self);
    int (*wait)(struct File* self);
    int (*cancel)(struct File* self);

    int (*reopen)(struct File* self, int access, int share, int flags);
    int (*type)(struct File* self);
    int (*fd)(struct File* self);

    int (*seek)(struct File* self, int from, int position);
    int (*lock)(struct File* self, int lock);

    struct File* (*duplicate)(struct File* self);
    struct File* (*accept)(struct File* self);
} File;

// file://{path}
// dir://{path}
// std://input
// std://output
// std://error
// pipe://{name}
// socket://{host}:{port}
// socket://{backlog}:{host}:{port}
struct File* file_new(char* uri);
void file_free(struct File* file, int remove);