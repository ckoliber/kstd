#include <memory/low/Type.h>

typedef struct Mutex {
    int acquire(struct Mutex* self, uint_64 timeout);
    int release(struct Mutex* self);
} Mutex;

Mutex* mutex_new(char* name);
void mutex_free(Mutex* mutex);