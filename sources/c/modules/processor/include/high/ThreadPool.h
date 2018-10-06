#include <memory/low/Type.h>

// vtable
typedef struct ThreadPool_VTable {
    int (*start)(struct ThreadPool* self);
    int (*post)(struct ThreadPool* self, void (*function)(void*), void* arg);
    int (*stop)(struct ThreadPool* self);
} ThreadPool_VTable;

// vtable + private data problem solve
typedef struct ThreadPool {
    ThreadPool_VTable* vtable;
} ThreadPool;

// init vtable
void threadpool_init();

// new raw threadpool
ThreadPool* threadpool_new();

// free raw threadpool
void threadpool_free(ThreadPool* threadpool);

// new threadpool
ThreadPool* threadpool_new_object(int size, tsize arg);