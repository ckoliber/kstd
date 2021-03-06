#include <low/Type.h>

// structs
typedef struct Share Share;
typedef struct Share_VTable Share_VTable;

// implement structs
// vtable + private data problem solve
struct Share {
    Share_VTable* vtable;
};

// vtable
struct Share_VTable {
    void* (*address)(Share* self);
    int (*connections)(Share* self);
    int (*flush)(Share* self, tsize size);
};

// init vtable
void share_init();

// new raw share
Share* share_new();

// free raw share
void share_free(Share* share);

// new share
Share* share_new_anonymous(tsize size, tsize offset);
Share* share_new_named(char* name, tsize size, tsize offset);
