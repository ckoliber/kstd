#include <low/Type.h>

// structs
typedef struct MutexLock MutexLock;
typedef struct MutexLock_VTable MutexLock_VTable;

// implement structs
// vtable + private data problem solve
struct MutexLock {
    MutexLock_VTable* vtable;
};

// vtable
struct MutexLock_VTable {
    int (*lock)(MutexLock* self, uint_64 timeout);
    int (*unlock)(MutexLock* self);
};

// init vtable
void mutexlock_init();

// new raw mutex lock
MutexLock* mutexlock_new();

// free raw mutex lock
void mutexlock_free(MutexLock* mutexlock);

// new mutex lock
MutexLock* mutexlock_new_object(char* name);