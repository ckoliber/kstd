#include <low/Type.h>

// structs
typedef struct ReadWriteLock ReadWriteLock;
typedef struct ReadWriteLock_VTable ReadWriteLock_VTable;

// implement structs
// vtable
struct ReadWriteLock_VTable {
    int (*read_lock)(ReadWriteLock* self, uint_64 timeout);
    int (*read_unlock)(ReadWriteLock* self);
    int (*write_lock)(ReadWriteLock* self, uint_64 timeout);
    int (*write_unlock)(ReadWriteLock* self);
};

// vtable + private data problem solve
struct ReadWriteLock {
    ReadWriteLock_VTable* vtable;
};

// init vtable
void readwritelock_init();

// new raw readwrite
ReadWriteLock* readwritelock_new();

// free raw readwrite
void readwritelock_free(ReadWriteLock* readwritelock);

// new readwrite
ReadWriteLock* readwritelock_new_object(char* name);