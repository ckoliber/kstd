#include <memory/low/Type.h>

typedef struct RWLock {
    SignedInt lock_read(struct RWLock* self, UnsignedLong timeout);
    SignedInt lock_write(struct RWLock* self, UnsignedLong timeout);
    SignedInt unlock_read(struct RWLock* self);
    SignedInt unlock_write(struct RWLock* self);
} RWLock;

RWLock* rwlock_new(Char* name);
Void rwlock_free(RWLock* rwlock);