#include <low/Type.h>

// structs
typedef struct Dequeue Dequeue;
typedef struct Dequeue_VTable Dequeue_VTable;

// implement structs
// vtable + private data problem solve
struct Dequeue {
    Dequeue_VTable* vtable;
};

// vtable
struct Dequeue_VTable {
    int (*enqueue)(Dequeue* self, int front, void* item, uint_64 timeout);
    void* (*dequeue)(Dequeue* self, int front, uint_64 timeout);
    void* (*get)(Dequeue* self, int front);
    int (*size)(Dequeue* self);
};

// init vtable
void dequeue_init();

// new raw dequeue
Dequeue* dequeue_new(int mode);

// free raw dequeue
void dequeue_free(Dequeue* dequeue);

// new dequeue
Dequeue* dequeue_new_object(int mode, int max, int (*comperator)(void*, void*));