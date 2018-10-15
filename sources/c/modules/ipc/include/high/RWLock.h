#include <low/Type.h>

// structs
typedef struct RWLock RWLock;
typedef struct RWLock_VTable RWLock_VTable;

// implement structs
// vtable
struct RWLock_VTable {
    int (*read_lock)(RWLock* self, uint_64 timeout);
    int (*read_unlock)(RWLock* self);
    int (*write_lock)(RWLock* self, uint_64 timeout);
    int (*write_unlock)(RWLock* self);
};

// vtable + private data problem solve
struct RWLock {
    RWLock_VTable* vtable;
};

// init vtable
void rwlock_init();

// new raw rwlock
RWLock* rwlock_new();

// free raw rwlock
void rwlock_free(RWLock* rwlock);

// new rwlock
RWLock* rwlock_new_object(int mode, char* name);