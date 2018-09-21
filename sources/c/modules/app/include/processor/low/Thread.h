#include <memory/low/Type.h>

typedef struct Thread {
    int (*start)(struct Thread* self, int (*function)(void*), void* arg);
    int (*join)(struct Thread* self);
    int (*id)(struct Thread* self);
    int (*stop)(struct Thread* self);
} Thread;

// init vtable
void pool_init();

// new raw thread
Thread* thread_new();

// free raw thread
void thread_free(Thread* thread);

// new thread
Thread* thread_new_object(tsize stack);