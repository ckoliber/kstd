#include <memory/low/Type.h>

typedef struct Lock {
    int (*lock)(struct Lock* self, uint_64 timeout);
    int (*unlock)(struct Lock* self);
} Lock;

Lock* lock_new(char* name);
void lock_free(Lock* lock);