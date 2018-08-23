#include <low/dsa/Queue.h>

#include <io/memory/Memory.h>
#include <low/itc/high/RWLock.h>
#include <low/itc/high/Semaphore.h>

struct Queue_ {
    struct Queue self;
    struct QueueItem* head;
    int size;
    struct RWLock* rwlock;
    struct Semaphore* semaphore;
    int (*comperator)(void* item1, void* item2);
};

struct QueueItem {
    struct QueueItem* next;
    struct QueueItem* previews;
    void* item;
};

// link methods
int queue_enqueue_normal(struct Queue* self, int front, void* item);
void* queue_dequeue_normal(struct Queue* self, int front, int timeout);
int queue_size_normal(struct Queue* self);

int queue_enqueue_concurrent(struct Queue* self, int front, void* item);
void* queue_dequeue_concurrent(struct Queue* self, int front, int timeout);
int queue_size_concurrent(struct Queue* self);

int queue_enqueue_blocking(struct Queue* self, int front, void* item);
void* queue_dequeue_blocking(struct Queue* self, int front, int timeout);
int queue_size_blocking(struct Queue* self);

int queue_enqueue_normal(struct Queue* self, int front, void* item) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // get item before target item
    // if queue_->comperator is not NULL then InsertionSort
    struct QueueItem* item_target = front ? queue_->head->next : queue_->head->previews;

    // allocate new queueitem and fill it
    struct QueueItem* queueitem = memory_alloc(sizeof(struct QueueItem));
    queueitem->item = item;
    queueitem->next = item_target->next;
    queueitem->previews = item_target;

    // add item to position
    int result = queue_->size;
    item_target->next->previews = queueitem;
    item_target->next = queueitem;
    queue_->size++;

    return result;
}
void* queue_dequeue_normal(struct Queue* self, int front, int timeout) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // check queue is not empty
    if (queue_->size <= 0) {
        return NULL;
    }

    // get target item
    struct QueueItem* item_target = front ? queue_->head->next : queue_->head->previews;

    // change removable item next and previews
    item_target->next->previews = item_target->previews;
    item_target->previews->next = item_target->next;

    // remove item
    void* result = item_target->item;
    memory_free(item_target);
    queue_->size--;

    return result;
}
int queue_size_normal(struct Queue* self) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // get queue size
    int result = queue_->size;

    return result;
}

int queue_enqueue_concurrent(struct Queue* self, int front, void* item) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // concurrent writelock
    queue_->rwlock->writelock(queue_->rwlock);

    // normal enqueue
    int result = queue_enqueue_normal(self, front, item);

    // concurrent writeunlock
    queue_->rwlock->writeunlock(queue_->rwlock);

    return result;
}
void* queue_dequeue_concurrent(struct Queue* self, int front, int timeout) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // concurrent writelock
    queue_->rwlock->writelock(queue_->rwlock);

    // normal dequeue
    void* result = queue_dequeue_normal(self, front, timeout);

    // concurrent writeunlock
    queue_->rwlock->writeunlock(queue_->rwlock);

    return result;
}
int queue_size_concurrent(struct Queue* self) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // concurrent readlock
    queue_->rwlock->readlock(queue_->rwlock);

    // normal size
    int result = queue_size_normal(self);

    // concurrent readunlock
    queue_->rwlock->readunlock(queue_->rwlock);

    return result;
}

int queue_enqueue_blocking(struct Queue* self, int front, void* item) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // normal enqueue
    int result = queue_enqueue_normal(self, front, item);

    // signal on semaphore
    queue_->semaphore->post(queue_->semaphore, 1);

    return result;
}
void* queue_dequeue_blocking(struct Queue* self, int front, int timeout) {
    struct Queue_* queue_ = (struct Queue_ *) self;

    // wait on semaphore
    queue_->semaphore->timewait(queue_->semaphore, 1, timeout);

    // normal dequeue
    void* result = queue_dequeue_normal(self, front, timeout);

    return result;
}
int queue_size_blocking(struct Queue* self) {

    // normal size
    int result = queue_size_normal(self);

    return result;
}

struct Queue* queue_new(int mode, int (*comperator)(void* item1, void* item2)) {
    struct Queue_* queue_ = memory_alloc(sizeof(struct Queue_));

    // init private methods
    switch (mode) {
        case 0:
            queue_->self.enqueue = queue_enqueue_normal;
            queue_->self.dequeue = queue_dequeue_normal;
            queue_->self.size = queue_size_normal;
            queue_->rwlock = NULL;
            queue_->semaphore = NULL;
            break;
        case 1:
            queue_->self.enqueue = queue_enqueue_concurrent;
            queue_->self.dequeue = queue_dequeue_concurrent;
            queue_->self.size = queue_size_concurrent;
            queue_->rwlock = rwlock_new();
            queue_->semaphore = NULL;
            break;
        case 2:
            queue_->self.enqueue = queue_enqueue_blocking;
            queue_->self.dequeue = queue_dequeue_blocking;
            queue_->self.size = queue_size_blocking;
            queue_->rwlock = NULL;
            queue_->semaphore = semaphore_new(0);
            break;
    }

    // init size and head and comperator
    queue_->size = 0;
    queue_->head = memory_alloc(sizeof(struct QueueItem));
    queue_->head->next = queue_->head;
    queue_->head->previews = queue_->head;
    queue_->head->item = NULL;
    queue_->comperator = comperator;

    return (struct Queue *) queue_;
}
void queue_free(struct Queue* queue) {
    struct Queue_* queue_ = (struct Queue_ *) queue;

    // break queue circle
    queue_->head->previews->next = NULL;

    // iterate queue and remove queueitems
    struct QueueItem* remove_item = NULL;
    do {
        remove_item = queue_->head;
        queue_->head = queue_->head->next;
        memory_free(remove_item);
    } while (queue_->head != NULL);

    // destry internal rwlock
    if (queue_->rwlock != NULL) {
        rwlock_free(queue_->rwlock);
    }

    // destroy internal semaphore
    if (queue_->semaphore != NULL) {
        semaphore_free(queue_->semaphore);
    }

    memory_free(queue_);
}