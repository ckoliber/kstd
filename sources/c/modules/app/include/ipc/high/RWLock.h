#include <memory/low/Type.h>

typedef struct RWLock {
    int (*read_lock)(struct RWLock* self, uint_64 timeout);
    int (*read_unlock)(struct RWLock* self);
    int (*write_lock)(struct RWLock* self, uint_64 timeout);
    int (*write_unlock)(struct RWLock* self);
} RWLock;

// init vtable
void rwlock_init();

// new raw rwlock
RWLock* rwlock_new();

// free raw rwlock
void rwlock_free(RWLock* rwlock);

// new rwlock
RWLock* rwlock_new_object(char* name);