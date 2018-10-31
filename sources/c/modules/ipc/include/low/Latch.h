#include <low/Type.h>

// structs
typedef struct Latch Latch;
typedef struct Latch_VTable Latch_VTable;

// implement structs
// vtable + private data problem solve
struct Latch {
    Latch_VTable* vtable;
};

// vtable
struct Latch_VTable {
    int (*wait)(Latch* self, uint_64 timeout);
    int (*down)(Latch* self);
    int (*get)(Latch* self);
};

// init vtable
void latch_init();

// new raw latch
Latch* latch_new();

// free raw latch
void latch_free(Latch* latch);

// new latch
Latch* latch_new_anonymous(int value);
Latch* latch_new_named(char* name, int value);
