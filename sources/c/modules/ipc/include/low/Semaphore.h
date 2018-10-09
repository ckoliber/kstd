#include <memory/low/Type.h>

// vtable
typedef struct Semaphore_VTable {
    int (*init)(struct Semaphore* self, int value);
    int (*wait)(struct Semaphore* self, uint_64 timeout);
    int (*post)(struct Semaphore* self);
    int (*get)(struct Semaphore* self);
} Semaphore_VTable;

// vtable + private data problem solve
typedef struct Semaphore {
    Semaphore_VTable* vtable;
} Semaphore;

// init vtable
void semaphore_init();

// new raw semaphore
Semaphore* semaphore_new();

// free raw semaphore
void semaphore_free(Semaphore* semaphore);

// new semaphore
Semaphore* semaphore_new_object(char* name);