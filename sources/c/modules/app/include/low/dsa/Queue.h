typedef struct Queue {
    int (*enqueue)(struct Queue* self, void* item);
    void* (*dequeue)(struct Queue* self, long int timeout);
    void* (*get)(struct Queue* self);
    int (*size)(struct Queue* self);
} Queue;

struct Queue * queue_new(int mode, int max, int (*comperator)(void*, void*));
void queue_free(struct Queue * queue);