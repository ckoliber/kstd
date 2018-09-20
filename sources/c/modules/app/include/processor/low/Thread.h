#include <memory/low/Type.h>

typedef struct Thread {
    int (*priority)(struct Thread* self, int priority);
    int (*affinity)(struct Thread* self, int affinity);
    int (*start)(struct Thread* self, int (*function)(void*), void* arg);
    int (*join)(struct Thread* self);
    int (*id)(struct Thread* self);
    int (*stop)(struct Thread* self);
} Thread;

Thread* thread_new(tsize stack);
void thread_free(Thread* thread);