#include <low/Type.h>

// structs
typedef struct ThreadPool ThreadPool;
typedef struct ThreadPool_VTable ThreadPool_VTable;

// implement structs
// vtable + private data problem solve
struct ThreadPool {
    ThreadPool_VTable* vtable;
};

// vtable
struct ThreadPool_VTable {
    int (*start)(ThreadPool* self);
    int (*post)(ThreadPool* self, void* item);
    int (*stop)(ThreadPool* self);
};

// init vtable
void threadpool_init();

// new raw threadpool
ThreadPool* threadpool_new();

// free raw threadpool
void threadpool_free(ThreadPool* threadpool);

// new threadpool
ThreadPool* threadpool_new_object(int size, tsize item);
