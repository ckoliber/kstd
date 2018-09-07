#include <memory/low/Type.h>

typedef struct Thread {
    int (*start)(struct Thread* self, void* (*function)(void*), void* arg);
    int (*join)(struct Thread* self);
    int (*id)(struct Thread* self);
    int (*stop)(struct Thread* self);
} Thread;

Thread* thread_new();
void thread_free(Thread* thread);