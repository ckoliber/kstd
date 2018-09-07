#include <memory/low/Type.h>

typedef struct Semaphore {
    int (*wait)(struct Semaphore* self, uint_64 timeout);
    int (*post)(struct Semaphore* self);
    int (*get)(struct Semaphore* self);
} Semaphore;

Semaphore* semaphore_new(char* name, int value);
void semaphore_free(Semaphore* semaphore);