#include <low/Type.h>

// vtable
typedef struct Queue_VTable {
    int (*enqueue)(struct Queue* self, void* item, uint_64 timeout);
    void* (*dequeue)(struct Queue* self, uint_64 timeout);
    void* (*get)(struct Queue* self);
    int (*size)(struct Queue* self);
} Queue_VTable;

// vtable + private data problem solve
typedef struct Queue {
    Queue_VTable* vtable;
} Queue;

// init vtable
void queue_init();

// new raw queue
Queue* queue_new(int mode);

// free raw queue
void queue_free(Queue* queue);

// new queue
Queue* queue_new_object(int mode, int max, int (*comperator)(void*, void*));