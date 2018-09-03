#include <memory/low/Type.h>

typedef struct Semaphore {
    SignedInt wait(struct Semaphore* self, UnsignedLong timeout);
    SignedInt post(struct Semaphore* self, UnsignedInt count);
    SignedInt get(struct Semaphore* self);
} Semaphore;

Semaphore* semaphore_new(Char* name);
Void semaphore_free(Semaphore* semaphore);