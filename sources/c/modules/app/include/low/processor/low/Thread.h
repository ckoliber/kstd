typedef struct Thread {
    int (*start)(struct Thread* self, void (*function)(void* arg), void* arg);
    int (*join)(struct Thread* self);
    int (*id)(struct Thread* self);
    int (*stop)(struct Thread* self);
} Thread;

struct Thread* thread_new();
void thread_free(struct Thread* thread);