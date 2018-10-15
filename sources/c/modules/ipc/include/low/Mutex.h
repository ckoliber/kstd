#include <low/Type.h>

// structs
typedef struct Mutex Mutex;
typedef struct Mutex_VTable Mutex_VTable;

// implement structs
// vtable + private data problem solve
struct Mutex {
    Mutex_VTable* vtable;
};

// vtable
struct Mutex_VTable {
    int (*acquire)(Mutex* self, uint_64 timeout);
    int (*release)(Mutex* self);
};

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