#include <memory/low/Type.h>

typedef struct Thread {
    int (*start)(struct Thread* self, int (*function)(void*), void* arg);
    int (*join)(struct Thread* self);
    int (*id)(struct Thread* self);
    int (*stop)(struct Thread* self);
} Thread;

Thread* thread_new(tsize stack);
void thread_free(Thread* thread);