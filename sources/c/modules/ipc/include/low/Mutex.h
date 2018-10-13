#include <low/Type.h>

// vtable
typedef struct Mutex_VTable {
    int (*acquire)(struct Mutex* self, uint_64 timeout);
    int (*release)(struct Mutex* self);
} Mutex_VTable;

// vtable + private data problem solve
typedef struct Mutex {
    Mutex_VTable* vtable;
} Mutex;

// init vtable
void mutex_init();

// new raw mutex
Mutex* mutex_new(int mode);

// free raw mutex
void mutex_free(Mutex* mutex);

// new mutex
Mutex* mutex_new_object(int mode, char* name);

// critical section global mutex
Mutex* critical;