#include <memory/low/Type.h>

typedef struct RWLock {
    int lock_read(struct RWLock* self, uint_64 timeout);
    int lock_write(struct RWLock* self, uint_64 timeout);
    int unlock_read(struct RWLock* self);
    int unlock_write(struct RWLock* self);
} RWLock;

RWLock* rwlock_new(char* name);
void rwlock_free(RWLock* rwlock);