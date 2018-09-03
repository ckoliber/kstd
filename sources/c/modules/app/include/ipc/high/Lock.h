#include <memory/low/Type.h>

typedef struct Lock {
    SignedInt lock(struct Lock* self, UnsignedLong timeout);
    SignedInt unlock(struct Lock* self);
} Lock;

Lock* lock_new(Char* name);
Void lock_free(Lock* lock);