#include <low/Type.h>

// structs
typedef struct Queue Queue;
typedef struct Queue_VTable Queue_VTable;

// implement structs
// vtable + private data problem solve
struct Queue {
    Queue_VTable* vtable;
};

// vtable
struct Queue_VTable {
    int (*enqueue)(Queue* self, uint_8* item, uint_64 timeout);
    uint_8* (*dequeue)(Queue* self, uint_64 timeout);
    uint_8* (*get)(Queue* self);
    int (*size)(Queue* self);
};

// init vtable
void queue_init();

// new raw queue
Queue* queue_new(int mode);

// free raw queue
void queue_free(Queue* queue);

// new queue
Queue* queue_new_object(int mode, int max, int (*comperator)(uint_8*, uint_8*));
