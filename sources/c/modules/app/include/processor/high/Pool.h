#include <memory/low/Type.h>

typedef struct Pool {
    int (*start)(struct Pool* self);
    int (*post)(struct Pool* self, void (*function)(void*), void* arg);
    int (*stop)(struct Pool* self);
} Pool;

// init vtable
void pool_init();

// new raw pool
Pool* pool_new();

// free raw pool
void pool_free(Pool* pool);

// new pool
Pool* pool_new_object(int size, tsize arg);