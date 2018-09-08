#include <dsa/high/Dequeue.h>

#include <ipc/high/RWLock.h>
#include <ipc/high/Semaphore.h>
#include <memory/low/Heap.h>

struct Dequeue_ {
    struct Dequeue self;
    struct DequeueItem* head;
    int size;
    int max;
    RWLock* rwlock;
    Semaphore* empty_semaphore;
    Semaphore* full_semaphore;
    int (*comperator)(void*, void*);
};

struct DequeueItem {
    struct DequeueItem* next;
    struct DequeueItem* previews;
    void* item;
};

// link methods
int dequeue_enqueue_normal(struct Dequeue* self, int front, void* item, uint_64 timeout);
void* dequeue_dequeue_normal(struct Dequeue* self, int front, uint_64 timeout);
void* dequeue_get_normal(struct Dequeue* self, int front);
int dequeue_size_normal(struct Dequeue* self);

int dequeue_enqueue_concurrent(struct Dequeue* self, int front, void* item, uint_64 timeout);
void* dequeue_dequeue_concurrent(struct Dequeue* self, int front, uint_64 timeout);
void* dequeue_get_concurrent(struct Dequeue* self, int front);
int dequeue_size_concurrent(struct Dequeue* self);

int dequeue_enqueue_blocking(struct Dequeue* self, int front, void* item, uint_64 timeout);
void* dequeue_dequeue_blocking(struct Dequeue* self, int front, uint_64 timeout);
void* dequeue_get_blocking(struct Dequeue* self, int front);
int dequeue_size_blocking(struct Dequeue* self);

// local methods
struct DequeueItem* dequeueitem_get(struct Dequeue* dequeue, int front, void* item);

struct DequeueItem* dequeueitem_get(struct Dequeue* dequeue, int front, void* item) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)dequeue;

    // return item before adding position
    struct DequeueItem* result = NULL;
    if (dequeue_->comperator != NULL) {
        // search for item position (insertion sort)
        result = dequeue_->head->next;
        while (result != dequeue_->head) {
            if (dequeue_->comperator(item, result->item) <= 0) {
                break;
            }
            result = result->next;
        }
        result = result->previews;
    } else if (front) {
        result = dequeue_->head;
    } else {
        result = dequeue_->head->previews;
    }

    return result;
}

int dequeue_enqueue_normal(struct Dequeue* self, int front, void* item, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // check dequeue is full
    if (dequeue_->max > 0 && dequeue_->size >= dequeue_->max) {
        return -1;
    }

    // get item before target item
    struct DequeueItem* item_target = dequeueitem_get(self, front, item);

    // allocate new dequeueitem and fill it
    struct DequeueItem* dequeueitem = heap_alloc(sizeof(struct DequeueItem));
    dequeueitem->item = item;
    dequeueitem->next = item_target->next;
    dequeueitem->previews = item_target;

    // add item to position
    int result = dequeue_->size;
    item_target->next->previews = dequeueitem;
    item_target->next = dequeueitem;
    dequeue_->size++;

    return result;
}
void* dequeue_dequeue_normal(struct Dequeue* self, int front, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // check dequeue is not empty
    if (dequeue_->size <= 0) {
        return NULL;
    }

    // get target item
    struct DequeueItem* item_target = front ? dequeue_->head->next : dequeue_->head->previews;

    // change removable item next and previews
    item_target->next->previews = item_target->previews;
    item_target->previews->next = item_target->next;

    // remove item
    void* result = item_target->item;
    heap_free(item_target);
    dequeue_->size--;

    return result;
}
void* dequeue_get_normal(struct Dequeue* self, int front) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // get dequeue item
    void* result = NULL;
    if (front) {
        result = dequeue_->head->next;
    } else {
        result = dequeue_->head->previews;
    }

    return result;
}
int dequeue_size_normal(struct Dequeue* self) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // get dequeue size
    int result = dequeue_->size;

    return result;
}

int dequeue_enqueue_concurrent(struct Dequeue* self, int front, void* item, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent writelock
    dequeue_->rwlock->write_lock(dequeue_->rwlock, UINT_64_MAX);

    // normal enqueue
    int result = dequeue_enqueue_normal(self, front, item);

    // concurrent writeunlock
    dequeue_->rwlock->write_unlock(dequeue_->rwlock);

    return result;
}
void* dequeue_dequeue_concurrent(struct Dequeue* self, int front, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent writelock
    dequeue_->rwlock->write_lock(dequeue_->rwlock, UINT_64_MAX);

    // normal dequeue
    void* result = dequeue_dequeue_normal(self, front, timeout);

    // concurrent writeunlock
    dequeue_->rwlock->write_unlock(dequeue_->rwlock);

    return result;
}
void* dequeue_get_concurrent(struct Dequeue* self, int front) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent readlock
    dequeue_->rwlock->read_lock(dequeue_->rwlock, UINT_64_MAX);

    // normal get
    void* result = dequeue_get_normal(self, front);

    // concurrent readunlock
    dequeue_->rwlock->read_unlock(dequeue_->rwlock);

    return result;
}
int dequeue_size_concurrent(struct Dequeue* self) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent readlock
    dequeue_->rwlock->read_lock(dequeue_->rwlock, UINT_64_MAX);

    // normal size
    int result = dequeue_size_normal(self);

    // concurrent readunlock
    dequeue_->rwlock->read_unlock(dequeue_->rwlock);

    return result;
}

int dequeue_enqueue_blocking(struct Dequeue* self, int front, void* item, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // wait on full semaphore
    if (dequeue_->full_semaphore != NULL) {
        dequeue_->full_semaphore->wait(dequeue_->full_semaphore, timeout);
    }

    // concurrent enqueue
    int result = dequeue_enqueue_concurrent(self, front, item);

    // signal on empty semaphore
    dequeue_->empty_semaphore->post(dequeue_->empty_semaphore);

    return result;
}
void* dequeue_dequeue_blocking(struct Dequeue* self, int front, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // wait on empty semaphore
    dequeue_->empty_semaphore->timewait(dequeue_->empty_semaphore, timeout);

    // concurrent dequeue
    void* result = dequeue_dequeue_concurrent(self, front, timeout);

    // signal on full semaphore
    if (dequeue_->full_semaphore != NULL) {
        dequeue_->full_semaphore->post(dequeue_->full_semaphore);
    }

    return result;
}
void* dequeue_get_blocking(struct Dequeue* self, int front) {
    // concurrent get
    void* result = dequeue_get_concurrent(self, front);

    return result;
}
int dequeue_size_blocking(struct Dequeue* self) {
    // concurrent size
    int result = dequeue_size_concurrent(self);

    return result;
}

Dequeue* dequeue_new(int mode, int max, int (*comperator)(void*, void*)) {
    struct Dequeue_* dequeue_ = heap_alloc(sizeof(struct Dequeue_));

    // init private methods
    switch (mode) {
        case 0:
            dequeue_->self.enqueue = dequeue_enqueue_normal;
            dequeue_->self.dequeue = dequeue_dequeue_normal;
            dequeue_->self.get = dequeue_get_normal;
            dequeue_->self.size = dequeue_size_normal;
            dequeue_->rwlock = NULL;
            dequeue_->empty_semaphore = NULL;
            dequeue_->full_semaphore = NULL;
            break;
        case 1:
            dequeue_->self.enqueue = dequeue_enqueue_concurrent;
            dequeue_->self.dequeue = dequeue_dequeue_concurrent;
            dequeue_->self.get = dequeue_get_concurrent;
            dequeue_->self.size = dequeue_size_concurrent;
            dequeue_->rwlock = rwlock_new(NULL);
            dequeue_->empty_semaphore = NULL;
            dequeue_->full_semaphore = NULL;
            break;
        case 2:
            dequeue_->self.enqueue = dequeue_enqueue_blocking;
            dequeue_->self.dequeue = dequeue_dequeue_blocking;
            dequeue_->self.get = dequeue_get_blocking;
            dequeue_->self.size = dequeue_size_blocking;
            dequeue_->rwlock = rwlock_new(NULL);
            dequeue_->empty_semaphore = semaphore_new(NULL, 0);
            dequeue_->full_semaphore = max > 0 ? semaphore_new(NULL, max) : NULL;
            break;
    }

    // init size and max and head and comperator
    dequeue_->size = 0;
    dequeue_->max = max;
    dequeue_->head = heap_alloc(sizeof(struct DequeueItem));
    dequeue_->head->next = dequeue_->head;
    dequeue_->head->previews = dequeue_->head;
    dequeue_->head->item = NULL;
    dequeue_->comperator = comperator;

    return (Dequeue*)dequeue_;
}
void dequeue_free(Dequeue* dequeue) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)dequeue;

    // break dequeue circle
    dequeue_->head->previews->next = NULL;

    // iterate dequeue and remove dequeueitems
    struct DequeueItem* remove_item = NULL;
    do {
        remove_item = dequeue_->head;
        dequeue_->head = dequeue_->head->next;
        heap_free(remove_item);
    } while (dequeue_->head != NULL);

    // destroy internal rwlock
    if (dequeue_->rwlock != NULL) {
        rwlock_free(dequeue_->rwlock);
    }

    // destroy internal semaphores
    if (dequeue_->empty_semaphore != NULL) {
        semaphore_free(dequeue_->empty_semaphore);
    }
    if (dequeue_->full_semaphore != NULL) {
        semaphore_free(dequeue_->full_semaphore);
    }

    heap_free(dequeue_);
}