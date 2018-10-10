#include <low/Type.h>

// vtable
typedef struct Lock_VTable {
    int (*lock)(struct Lock* self, uint_64 timeout);
    int (*unlock)(struct Lock* self);
} Lock_VTable;

// vtable + private data problem solve
typedef struct Lock {
    Lock_VTable* vtable;
} Lock;

// init vtable
void lock_init();

// new raw lock
Lock* lock_new();

// free raw lock
void lock_free(Lock* lock);

// new lock
Lock* lock_new_object(int mode, char* name);