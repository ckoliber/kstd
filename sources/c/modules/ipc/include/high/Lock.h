#include <low/Type.h>

// structs
typedef struct Lock Lock;
typedef struct Lock_VTable Lock_VTable;

// implement structs
// vtable + private data problem solve
struct Lock {
    Lock_VTable* vtable;
};

// vtable
struct Lock_VTable {
    int (*lock)(Lock* self, uint_64 timeout);
    int (*unlock)(Lock* self);
};

// init vtable
void lock_init();

// new raw lock
Lock* lock_new();

// free raw lock
void lock_free(Lock* lock);

// new lock
Lock* lock_new_object(int mode, char* name);