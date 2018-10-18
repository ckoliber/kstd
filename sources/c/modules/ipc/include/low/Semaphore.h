#include <low/Type.h>

// structs
typedef struct Semaphore Semaphore;
typedef struct Semaphore_VTable Semaphore_VTable;

// implement structs
// vtable + private data problem solve
struct Semaphore {
    Semaphore_VTable* vtable;
};

// vtable
struct Semaphore_VTable {
    int (*wait)(Semaphore* self, uint_64 timeout);
    int (*post)(Semaphore* self);
    int (*get)(Semaphore* self);
};

// init vtable
void semaphore_init();

// new raw semaphore
Semaphore* semaphore_new();

// free raw semaphore
void semaphore_free(Semaphore* semaphore);

// new semaphore
Semaphore* semaphore_new_object(char* name, int value);
