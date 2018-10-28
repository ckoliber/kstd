#include <low/Type.h>

// structs
typedef struct ReentrantLock ReentrantLock;
typedef struct ReentrantLock_VTable ReentrantLock_VTable;

// implement structs
// vtable + private data problem solve
struct ReentrantLock {
    ReentrantLock_VTable* vtable;
};

// vtable
struct ReentrantLock_VTable {
    int (*lock)(ReentrantLock* self, uint_64 timeout);
    int (*unlock)(ReentrantLock* self);
};

// init vtable
void reentrantlock_init();

// new raw reentrant lock
ReentrantLock* reentrantlock_new();

// free raw reentrant lock
void reentrantlock_free(ReentrantLock* reentrantlock);

// new reentrant lock
ReentrantLock* reentrantlock_new_anonymous();
ReentrantLock* reentrantlock_new_named(char* name);

// public critical variable
ReentrantLock* critical;
