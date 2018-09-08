#include <memory/low/Type.h>

typedef struct ThreadPool {
    int (*start)(struct ThreadPool* self);
    int (*post)(struct ThreadPool* self, void (*function)(void*), void* arg);
    int (*stop)(struct ThreadPool* self, bool force);
} ThreadPool;

ThreadPool* threadpool_new(int size, tsize arg);
void threadpool_free(ThreadPool* threadpool);