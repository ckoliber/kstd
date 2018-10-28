#include <high/Dequeue.h>

#include <low/ReadWriteLock.h>
#include <low/Heap.h>
#include <low/Semaphore.h>

struct Dequeue_ {
    // self public object
    Dequeue self;

    // constructor data
    int max;
    int (*comperator)(void*, void*);

    // private data
    int size;
    struct DequeueItem* head;
    ReadWriteLock* readwritelock;
    Semaphore* full_semaphore;
    Semaphore* empty_semaphore;
};

struct DequeueItem {
    struct DequeueItem* next;
    struct DequeueItem* previews;
    void* item;
};

// vtable
Dequeue_VTable* dequeue_vtable_normal;
Dequeue_VTable* dequeue_vtable_concurrent;
Dequeue_VTable* dequeue_vtable_blocking;

// link methods
int dequeue_enqueue_normal(Dequeue* self, int front, void* item, uint_64 timeout);
void* dequeue_dequeue_normal(Dequeue* self, int front, uint_64 timeout);
void* dequeue_get_normal(Dequeue* self, int front);
int dequeue_size_normal(Dequeue* self);

int dequeue_enqueue_concurrent(Dequeue* self, int front, void* item, uint_64 timeout);
void* dequeue_dequeue_concurrent(Dequeue* self, int front, uint_64 timeout);
void* dequeue_get_concurrent(Dequeue* self, int front);
int dequeue_size_concurrent(Dequeue* self);

int dequeue_enqueue_blocking(Dequeue* self, int front, void* item, uint_64 timeout);
void* dequeue_dequeue_blocking(Dequeue* self, int front, uint_64 timeout);
void* dequeue_get_blocking(Dequeue* self, int front);
int dequeue_size_blocking(Dequeue* self);

// local methods
struct DequeueItem* dequeueitem_get(Dequeue* dequeue, int front, void* item);

// implement methods
struct DequeueItem* dequeueitem_get(Dequeue* dequeue, int front, void* item) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)dequeue;

    // return item before adding position
    if (dequeue_->comperator != NULL) {
        // search for item position (insertion sort)
        struct DequeueItem* temp = dequeue_->head->next;
        while (temp != dequeue_->head) {
            if (dequeue_->comperator(item, temp->item) <= 0) {
                break;
            }
            temp = temp->next;
        }
        return temp->previews;
    } else if (front) {
        return dequeue_->head;
    } else {
        return dequeue_->head->previews;
    }
}

// normal mode vtable operators
int dequeue_enqueue_normal(Dequeue* self, int front, void* item, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // check dequeue is full
    if (dequeue_->max > 0 && dequeue_->size >= dequeue_->max) {
        return -1;
    }

    // get item before target item
    struct DequeueItem* item_target = dequeueitem_get(self, front, item);

    // allocate new dequeueitem and fill it
    struct DequeueItem* dequeueitem = (struct DequeueItem*) heap_alloc(sizeof(struct DequeueItem));
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
void* dequeue_dequeue_normal(Dequeue* self, int front, uint_64 timeout) {
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
void* dequeue_get_normal(Dequeue* self, int front) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // get dequeue item
    if (front) {
        return dequeue_->head->next->item;
    } else {
        return dequeue_->head->previews->item;
    }
}
int dequeue_size_normal(Dequeue* self) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // get dequeue size
    int result = dequeue_->size;

    return result;
}

// concurrent mode vtable operators
int dequeue_enqueue_concurrent(Dequeue* self, int front, void* item, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent writelock
    dequeue_->readwritelock->vtable->write_lock(dequeue_->readwritelock, UINT_64_MAX);

    // normal enqueue
    int result = dequeue_enqueue_normal(self, front, item, timeout);

    // concurrent writeunlock
    dequeue_->readwritelock->vtable->write_unlock(dequeue_->readwritelock);

    return result;
}
void* dequeue_dequeue_concurrent(Dequeue* self, int front, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent writelock
    dequeue_->readwritelock->vtable->write_lock(dequeue_->readwritelock, UINT_64_MAX);

    // normal dequeue
    void* result = dequeue_dequeue_normal(self, front, timeout);

    // concurrent writeunlock
    dequeue_->readwritelock->vtable->write_unlock(dequeue_->readwritelock);

    return result;
}
void* dequeue_get_concurrent(Dequeue* self, int front) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent readlock
    dequeue_->readwritelock->vtable->read_lock(dequeue_->readwritelock, UINT_64_MAX);

    // normal get
    void* result = dequeue_get_normal(self, front);

    // concurrent readunlock
    dequeue_->readwritelock->vtable->read_unlock(dequeue_->readwritelock);

    return result;
}
int dequeue_size_concurrent(Dequeue* self) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // concurrent readlock
    dequeue_->readwritelock->vtable->read_lock(dequeue_->readwritelock, UINT_64_MAX);

    // normal size
    int result = dequeue_size_normal(self);

    // concurrent readunlock
    dequeue_->readwritelock->vtable->read_unlock(dequeue_->readwritelock);

    return result;
}

// blocking mode vtable operators
int dequeue_enqueue_blocking(Dequeue* self, int front, void* item, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // wait on full semaphore
    if (dequeue_->full_semaphore != NULL) {
        dequeue_->full_semaphore->vtable->wait(dequeue_->full_semaphore, timeout);
    }

    // concurrent enqueue
    int result = dequeue_enqueue_concurrent(self, front, item, timeout);

    // signal on empty semaphore
    if (dequeue_->empty_semaphore != NULL) {
        dequeue_->empty_semaphore->vtable->post(dequeue_->empty_semaphore);
    }

    return result;
}
void* dequeue_dequeue_blocking(Dequeue* self, int front, uint_64 timeout) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)self;

    // wait on empty semaphore
    if (dequeue_->empty_semaphore != NULL) {
        dequeue_->empty_semaphore->vtable->wait(dequeue_->empty_semaphore, timeout);
    }

    // concurrent dequeue
    void* result = dequeue_dequeue_concurrent(self, front, timeout);

    // signal on full semaphore
    if (dequeue_->full_semaphore != NULL) {
        dequeue_->full_semaphore->vtable->post(dequeue_->full_semaphore);
    }

    return result;
}
void* dequeue_get_blocking(Dequeue* self, int front) {
    // concurrent get
    void* result = dequeue_get_concurrent(self, front);

    return result;
}
int dequeue_size_blocking(Dequeue* self) {
    // concurrent size
    int result = dequeue_size_concurrent(self);

    return result;
}

// object allocation and deallocation operators
void dequeue_init() {
    // init normal vtable
    dequeue_vtable_normal = heap_alloc(sizeof(Dequeue_VTable));
    dequeue_vtable_normal->enqueue = dequeue_enqueue_normal;
    dequeue_vtable_normal->dequeue = dequeue_dequeue_normal;
    dequeue_vtable_normal->get = dequeue_get_normal;
    dequeue_vtable_normal->size = dequeue_size_normal;

    // init concurrent vtable
    dequeue_vtable_concurrent = heap_alloc(sizeof(Dequeue_VTable));
    dequeue_vtable_concurrent->enqueue = dequeue_enqueue_concurrent;
    dequeue_vtable_concurrent->dequeue = dequeue_dequeue_concurrent;
    dequeue_vtable_concurrent->get = dequeue_get_concurrent;
    dequeue_vtable_concurrent->size = dequeue_size_concurrent;

    // init blocking vtable
    dequeue_vtable_blocking = heap_alloc(sizeof(Dequeue_VTable));
    dequeue_vtable_blocking->enqueue = dequeue_enqueue_blocking;
    dequeue_vtable_blocking->dequeue = dequeue_dequeue_blocking;
    dequeue_vtable_blocking->get = dequeue_get_blocking;
    dequeue_vtable_blocking->size = dequeue_size_blocking;
}
Dequeue* dequeue_new(int mode) {
    struct Dequeue_* dequeue_ = heap_alloc(sizeof(struct Dequeue_));

    // set vtable
    switch (mode) {
        case 0:
            dequeue_->self.vtable = dequeue_vtable_normal;
            break;
        case 1:
            dequeue_->self.vtable = dequeue_vtable_concurrent;
            break;
        case 2:
            dequeue_->self.vtable = dequeue_vtable_blocking;
            break;
    }

    // set constructor data
    dequeue_->max = 0;
    dequeue_->comperator = NULL;

    // set private data
    dequeue_->size = 0;
    dequeue_->head = NULL;
    dequeue_->readwritelock = NULL;
    dequeue_->full_semaphore = NULL;
    dequeue_->empty_semaphore = NULL;

    return (Dequeue*)dequeue_;
}
void dequeue_free(Dequeue* dequeue) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)dequeue;

    // free private data

    // iterate dequeue and remove dequeueitems
    struct DequeueItem* remove_item = NULL;

    // break circular linked dequeue
    dequeue_->head->previews->next = NULL;
    do {
        remove_item = dequeue_->head;
        dequeue_->head = dequeue_->head->next;
        heap_free(remove_item);
    } while (dequeue_->head != NULL);

    if (dequeue_->readwritelock != NULL) {
        readwritelock_free(dequeue_->readwritelock);
    }
    if (dequeue_->full_semaphore != NULL) {
        semaphore_free(dequeue_->full_semaphore);
    }
    if (dequeue_->empty_semaphore != NULL) {
        semaphore_free(dequeue_->empty_semaphore);
    }

    // free self
    heap_free(dequeue_);
}
Dequeue* dequeue_new_object(int mode, int max, int (*comperator)(void*, void*)) {
    struct Dequeue_* dequeue_ = (struct Dequeue_*)dequeue_new(mode);

    // set constructor data
    dequeue_->max = max;
    dequeue_->comperator = comperator;

    // set private data
    dequeue_->size = 0;

    // init head
    dequeue_->head = heap_alloc(sizeof(struct DequeueItem));
    dequeue_->head->next = dequeue_->head;
    dequeue_->head->previews = dequeue_->head;
    dequeue_->head->item = NULL;

    if (mode == 1 || mode == 2) {
        dequeue_->readwritelock = readwritelock_new_anonymous();
    }
    if (mode == 2) {
        // init empty semaphore
        dequeue_->empty_semaphore = semaphore_new_anonymous(0);

        // init full semaphore
        if (max > 0) {
            dequeue_->full_semaphore = semaphore_new_anonymous(max);
        }
    }

    return (Dequeue*)dequeue_;
}
