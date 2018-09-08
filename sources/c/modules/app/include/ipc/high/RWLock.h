#include <memory/low/Type.h>

typedef struct RWLock {
    int (*read_lock)(struct RWLock* self, uint_64 timeout);
    int (*read_unlock)(struct RWLock* self);
    int (*write_lock)(struct RWLock* self, uint_64 timeout);
    int (*write_unlock)(struct RWLock* self);
} RWLock;

RWLock* rwlock_new(char* name);
void rwlock_free(RWLock* rwlock);