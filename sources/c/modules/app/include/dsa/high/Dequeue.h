#include <memory/low/Type.h>

typedef struct Dequeue {
    int (*enqueue)(struct Dequeue* self, int front, void* item, uint_64 timeout);
    void* (*dequeue)(struct Dequeue* self, int front, uint_64 timeout);
    void* (*get)(struct Dequeue* self, int front);
    int (*size)(struct Dequeue* self);
} Dequeue;

Dequeue* dequeue_new(int mode, int max, int (*comperator)(void*, void*));
void dequeue_free(Dequeue* dequeue);