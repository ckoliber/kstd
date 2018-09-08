#include <memory/low/Type.h>

typedef struct Queue {
    int (*enqueue)(struct Queue* self, void* item, uint_64 timeout);
    void* (*dequeue)(struct Queue* self, uint_64 timeout);
    void* (*get)(struct Queue* self);
    int (*size)(struct Queue* self);
} Queue;

Queue* queue_new(int mode, int max, int (*comperator)(void*, void*));
void queue_free(Queue* queue);