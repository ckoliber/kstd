#include <low/Type.h>

// structs
typedef struct Barrier Barrier;
typedef struct Barrier_VTable Barrier_VTable;

// implement structs
// vtable + private data problem solve
struct Barrier {
    Barrier_VTable* vtable;
};

// vtable
struct Barrier_VTable {
    int (*wait)(Barrier* self, uint_64 timeout);
    int (*get)(Barrier* self);
};

// init vtable
void barrier_init();

// new raw barrier
Barrier* barrier_new();

// free raw barrier
void barrier_free(Barrier* barrier);

// new barrier
Barrier* barrier_new_anonymous(int value);
Barrier* barrier_new_named(char* name, int value);
