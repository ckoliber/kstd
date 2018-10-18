#include <low/Type.h>

// structs
typedef struct ErrorCheckLock ErrorCheckLock;
typedef struct ErrorCheckLock_VTable ErrorCheckLock_VTable;

// implement structs
// vtable + private data problem solve
struct ErrorCheckLock {
    ErrorCheckLock_VTable* vtable;
};

// vtable
struct ErrorCheckLock_VTable {
    int (*lock)(ErrorCheckLock* self, uint_64 timeout);
    int (*unlock)(ErrorCheckLock* self);
};

// init vtable
void errorchecklock_init();

// new raw errorcheck lock
ErrorCheckLock* errorchecklock_new();

// free raw errorcheck lock
void errorchecklock_free(ErrorCheckLock* errorchecklock);

// new errorcheck lock
ErrorCheckLock* errorchecklock_new_object(char* name);
