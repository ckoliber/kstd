#include <high/Queue.h>

#include <high/Dequeue.h>
#include <low/Heap.h>

struct Queue_ {
    // self public object
    Queue self;

    // constructor data

    // private data
    Dequeue* dequeue;
};

// vtable
Queue_VTable* queue_vtable;

// link methods
int queue_enqueue(struct Queue* self, void* item, uint_64 timeout);
void* queue_dequeue(struct Queue* self, uint_64 timeout);
void* queue_get(struct Queue* self);
int queue_size(struct Queue* self);

// implement methods
// vtable operators
int queue_enqueue(struct Queue* self, void* item, uint_64 timeout) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // internal dequeue enqueue
    int result = queue_->dequeue->vtable->enqueue(queue_->dequeue, 1, item, timeout);

    return result;
}
void* queue_dequeue(struct Queue* self, uint_64 timeout) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // internal dequeue dequeue
    void* result = queue_->dequeue->vtable->dequeue(queue_->dequeue, 0, timeout);

    return result;
}
void* queue_get(struct Queue* self) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // internal dequeue get
    void* result = queue_->dequeue->vtable->get(queue_->dequeue, 0);

    return result;
}
int queue_size(struct Queue* self) {
    struct Queue_* queue_ = (struct Queue_*)self;

    // internal dequeue size
    int result = queue_->dequeue->vtable->size(queue_->dequeue);

    return result;
}

// object allocation and deallocation operators
void queue_init() {
    // init normal vtable
    queue_vtable = heap_alloc(sizeof(Queue_VTable));
    queue_vtable->enqueue = queue_enqueue;
    queue_vtable->dequeue = queue_dequeue;
    queue_vtable->get = queue_get;
    queue_vtable->size = queue_size;
}
Queue* queue_new(int mode) {
    struct Queue_* queue_ = heap_alloc(sizeof(struct Queue_));

    // set vtable
    queue_->self.vtable = queue_vtable;

    // set constructor data

    // set private data
    queue_->dequeue = NULL;

    return (Queue*)queue_;
}
void queue_free(Queue* queue) {
    struct Queue_* queue_ = (struct Queue_*)queue;

    // free private data
    if (queue_->dequeue != NULL) {
        dequeue_free(queue_->dequeue);
    }

    // free self
    heap_free(queue_);
}
Queue* queue_new_object(int mode, int max, int (*comperator)(void*, void*)) {
    struct Queue_* queue_ = (struct Queue_*)queue_new(mode);

    // set constructor data

    // set private data
    queue_->dequeue = dequeue_new_object(mode, max, comperator);

    return (Queue*)queue_;
}