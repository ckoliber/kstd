#include <memory/low/Type.h>

typedef struct Thread {
    SignedInt (*start)(struct Thread* self, Void* (*function)(Void*), Void* arg);
    SignedInt (*join)(struct Thread* self);
    SignedInt (*id)(struct Thread* self);
    SignedInt (*stop)(struct Thread* self);
} Thread;

Thread* thread_new();
void thread_free(Thread* thread);