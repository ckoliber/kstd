#include <memory/low/Type.h>

typedef struct Mutex {
    int (*acquire)(struct Mutex* self, uint_64 timeout);
    int (*release)(struct Mutex* self);
} Mutex;

// init vtable
void mutex_init();

// new raw mutex
Mutex* mutex_new();

// free raw mutex
void mutex_free(Mutex* mutex);

// new mutex
Mutex* mutex_new_object(int mode, char* name);

// critical section global mutex
Mutex* critical = NULL;