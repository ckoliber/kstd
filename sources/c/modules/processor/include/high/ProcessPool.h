#include <low/Type.h>

// vtable
typedef struct ProcessPool_VTable {
    int (*start)(struct ProcessPool* self);
    int (*post)(struct ProcessPool* self, void (*function)(void*), void* arg);
    int (*stop)(struct ProcessPool* self);
} ProcessPool_VTable;

// vtable + private data problem solve
typedef struct ProcessPool {
    ProcessPool_VTable* vtable;
} ProcessPool;

// init vtable
void processpool_init();

// new raw processpool
ProcessPool* processpool_new();

// free raw processpool
void processpool_free(ProcessPool* processpool);

// new processpool
ProcessPool* processpool_new_object(int size, tsize arg);