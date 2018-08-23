typedef struct Dequeue {
    int (*enqueue)(struct Dequeue* self, int front, void* item);
    void* (*dequeue)(struct Dequeue* self, int front, long int timeout);
    void* (*get)(struct Dequeue* self, int front);
    int (*size)(struct Dequeue* self);
} Dequeue;

struct Dequeue* dequeue_new(int mode, int max, int (*comperator)(void*, void*));
void dequeue_free(struct Dequeue* dequeue);