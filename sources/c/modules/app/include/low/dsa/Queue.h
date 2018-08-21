typedef struct Queue {
    int (*enqueue)(struct Queue* self, int front, void* item);
    void* (*dequeue)(struct Queue* self, int front);
    void* (*blockdequeue)(struct Queue* self, int front);
    void* (*timeblockdequeue)(struct Queue* self, int front, int timeout);
    int (*size)(struct Queue* self);
} Queue;

struct Queue* queue_new(int concurrent, int (*comperator)(void* item1, void* item2));
void queue_free(struct Queue* queue);