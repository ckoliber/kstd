#include <low/dsa/Queue.h>

#include <io/memory/Memory.h>
#include <low/dsa/Dequeue.h>

struct Queue_ {
    struct Queue self;
    struct Dequeue* dequeue;
};

// link methods
int queue_enqueue(struct Queue* self, void* item);
void* queue_dequeue(struct Queue* self, long int timeout);
void* queue_get(struct Queue* self);
int queue_size(struct Queue* self);

int queue_enqueue(struct Queue* self, void* item) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // Dequeue enqueue to front
    int result = queue_->dequeue->enqueue(queue_->dequeue, 1, item);

    return result;
}
void* queue_dequeue(struct Queue* self, long int timeout) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // Dequeue dequeue from back
    void* result = queue_->dequeue->dequeue(queue_->dequeue, 0, timeout);

    return result;
}
void* queue_get(struct Queue* self) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // Dequeue get from back
    void* result = queue_->dequeue->get(queue_->dequeue, 0);

    return result;
}
int queue_size(struct Queue* self) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // Dequeue get size
    int result = queue_->dequeue->size(queue_->dequeue);

    return result;
}

struct Queue* queue_new(int mode, int max, int (*comperator)(void*, void*)) {
    struct Queue_* queue_ = memory_alloc(sizeof(struct Queue_));

    // init private methods
    queue_->self.enqueue = queue_enqueue;
    queue_->self.dequeue = queue_dequeue;
    queue_->self.get = queue_get;
    queue_->self.size = queue_size;

    // init internal Dequeue
    queue_->dequeue = dequeue_new(mode, max, comperator);

    return (struct Queue*)queue_;
}
void queue_free(struct Queue* queue) {
    struct Queue_* queue_ = (struct Queue_*)queue;

    // destroy internal Dequeue
    dequeue_free(queue_->dequeue);

    memory_free(queue_);
}