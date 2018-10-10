#include <low/Type.h>

// vtable
typedef struct RWLock_VTable {
    int (*read_lock)(struct RWLock* self, uint_64 timeout);
    int (*read_unlock)(struct RWLock* self);
    int (*write_lock)(struct RWLock* self, uint_64 timeout);
    int (*write_unlock)(struct RWLock* self);
} RWLock_VTable;

// vtable + private data problem solve
typedef struct RWLock {
    RWLock_VTable* vtable;
} RWLock;

// init vtable
void rwlock_init();

// new raw rwlock
RWLock* rwlock_new();

// free raw rwlock
void rwlock_free(RWLock* rwlock);

// new rwlock
RWLock* rwlock_new_object(int mode, char* name);