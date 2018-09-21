#include <memory/low/Type.h>

typedef struct Semaphore {
    int (*init)(struct Semaphore* self, int value);
    int (*wait)(struct Semaphore* self, uint_64 timeout);
    int (*post)(struct Semaphore* self);
    int (*get)(struct Semaphore* self);
} Semaphore;

// init vtable
void semaphore_init();

// new raw semaphore
Semaphore* semaphore_new();

// free raw semaphore
void semaphore_free(Semaphore* semaphore);

// new semaphore
Semaphore* semaphore_new_object(char* name);