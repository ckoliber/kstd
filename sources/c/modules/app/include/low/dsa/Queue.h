typedef struct Queue {
    int enqueue(int front, void* item);
    void* dequeue(int front);
    void* blockdequeue(int front);
    void* tryblockdequeue(int front, int timeout);
    int size();
} Queue;

struct Queue* queue_new(int concurrent, int (*comperator)(void* item1, void* item2));
void queue_free(struct Queue* queue);