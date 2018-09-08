#include <memory/low/Type.h>

typedef struct ProcessPool {
    int (*start)(struct ProcessPool* self);
    int (*post)(struct ProcessPool* self, void (*function)(void*), void* arg);
    int (*stop)(struct ProcessPool* self, bool force);
} ProcessPool;

ProcessPool* processpool_new(int size, tsize arg);
void processpool_free(ProcessPool* processpool);