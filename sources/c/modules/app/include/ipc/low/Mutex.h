#include <memory/low/Type.h>

typedef struct Mutex {
    SignedInt acquire(struct Mutex* self, UnsignedLong timeout);
    SignedInt release(struct Mutex* self);
} Mutex;

Mutex* mutex_new(Char* name);
Void mutex_free(Mutex* mutex);