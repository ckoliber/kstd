#include <memory/low/Type.h>

// vtable
typedef struct Poller_VTable {
    int (*add)(struct Poller* self, struct File* fd, int access);
    int (*remove)(struct Poller* self, struct File* fd);
    int (*wait)(struct Poller* self, struct File* fd, uint_64 timeout);

    // int (*size)(struct Poller* self, struct File* fd);
    // int (*get)(struct Poller* self, struct File* fd);
} Poller_VTable;

// vtable + private data problem solve
typedef struct Poller {
    Poller_VTable* vtable;
} Poller;

// init vtable
void poller_init();

// new raw poller
Poller* poller_new();

// free raw poller
void poller_free(Poller* poller);

// new poller
Poller* poller_new_object(int max);