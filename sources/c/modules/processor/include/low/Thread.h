#include <low/Type.h>

// structs
typedef struct Thread Thread;
typedef struct Thread_VTable Thread_VTable;

// implement structs
// vtable + private data problem solve
struct Thread {
    Thread_VTable* vtable;
};

// vtable
struct Thread_VTable {
    int (*start)(Thread* self, int (*function)(void*), void* arg);
    int (*join)(Thread* self);
    int (*id)(Thread* self);
    int (*stop)(Thread* self);
};

// init vtable
void thread_init();

// new raw thread
Thread* thread_new();

// free raw thread
void thread_free(Thread* thread);

// new thread
Thread* thread_new_object(tsize stack);

// local thread methods
int thread_get_self();