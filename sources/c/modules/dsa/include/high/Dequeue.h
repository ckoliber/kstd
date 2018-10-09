#include <low/Type.h>

// vtable
typedef struct Dequeue_VTable {
    int (*enqueue)(struct Dequeue* self, int front, void* item, uint_64 timeout);
    void* (*dequeue)(struct Dequeue* self, int front, uint_64 timeout);
    void* (*get)(struct Dequeue* self, int front);
    int (*size)(struct Dequeue* self);
} Dequeue_VTable;

// vtable + private data problem solve
typedef struct Dequeue {
    Dequeue_VTable* vtable;
} Dequeue;

// init vtable
void dequeue_init();

// new raw dequeue
Dequeue* dequeue_new(int mode);

// free raw dequeue
void dequeue_free(Dequeue* dequeue);

// new dequeue
Dequeue* dequeue_new_object(int mode, int max, int (*comperator)(void*, void*));