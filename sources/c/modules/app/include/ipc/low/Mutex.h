#include <memory/low/Type.h>

typedef struct Mutex {
    int (*acquire)(struct Mutex* self, uint_64 timeout);
    int (*release)(struct Mutex* self);
} Mutex;

// critical section global mutex
Mutex* critical = NULL;

Mutex* mutex_new(int mode, char* name);
void mutex_free(Mutex* mutex);