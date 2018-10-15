#include <memory/low/Type.h>

// structs
typedef struct Poller Poller;
typedef struct Poller_VTable Poller_VTable;

// implement structs
// vtable + private data problem solve
struct Poller {
    Poller_VTable* vtable;
};

// vtable
struct Poller_VTable {
    int (*add)(Poller* self, File* fd, int access);
    int (*remove)(Poller* self, File* fd);
    int (*wait)(Poller* self, File* fd, uint_64 timeout);

    // int (*size)(Poller* self, struct File* fd);
    // int (*get)(Poller* self, struct File* fd);
};

// init vtable
void poller_init();

// new raw poller
Poller* poller_new();

// free raw poller
void poller_free(Poller* poller);

// new poller
Poller* poller_new_object(int max);