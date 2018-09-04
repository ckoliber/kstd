#include <memory/low/Type.h>

typedef struct Thread {
    SignedInt (*start)(struct Thread* self, void* (*function)(void*), void* arg);
    Void* (*join)(struct Thread* self);
    SignedInt (*id)(struct Thread* self);
    SignedInt (*stop)(struct Thread* self);
} Thread;

struct Thread* thread_new();
void thread_free(struct Thread* thread);