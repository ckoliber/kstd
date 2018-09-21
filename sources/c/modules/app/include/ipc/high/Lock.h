#include <memory/low/Type.h>

typedef struct Lock {
    int (*lock)(struct Lock* self, uint_64 timeout);
    int (*unlock)(struct Lock* self);
} Lock;

// init vtable
void lock_init();

// new raw lock
Lock* lock_new();

// free raw lock
void lock_free(Lock* lock);

// new lock
Lock* lock_new_object(int mode, char* name);