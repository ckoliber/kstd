#include <low/Type.h>

// structs
typedef struct ProcessPool ProcessPool;
typedef struct ProcessPool_VTable ProcessPool_VTable;

// implement structs
// vtable + private data problem solve
struct ProcessPool {
    ProcessPool_VTable* vtable;
};

// vtable
struct ProcessPool_VTable {
    int (*start)(ProcessPool* self);
    int (*post)(ProcessPool* self, void* item);
    int (*stop)(ProcessPool* self);
};

// init vtable
void processpool_init();

// new raw processpool
ProcessPool* processpool_new();

// free raw processpool
void processpool_free(ProcessPool* processpool);

// new processpool
ProcessPool* processpool_new_object(int size, tsize item);
