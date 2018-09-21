#include <dsa/high/LinkedList.h>

#include <ipc/high/RWLock.h>
#include <memory/low/Heap.h>

struct LinkedList_ {
    // self public object
    LinkedList self;

    // constructor data
    int (*comperator)(void*, void*);

    // private data
    int size;
    struct LinkedItem* head;
    RWLock* rwlock;
};

struct LinkedItem {
    struct LinkedItem* next;
    struct LinkedItem* previews;
    void* item;
};

struct LinkedListIterator_ {
    // self public object
    LinkedListIterator self;

    // constructor data

    // private data
    struct LinkedItem* item;
    struct LinkedItem* end;
};

// vtable
LinkedList_VTable* linkedlist_vtable_normal;
LinkedList_VTable* linkedlist_vtable_concurrent;

LinkedListIterator_VTable* linkedlistiterator_vtable;

// link methods
int linkedlist_add_normal(struct LinkedList* self, void* item);
int linkedlist_addto_normal(struct LinkedList* self, int position, void* item);
void* linkedlist_put_normal(struct LinkedList* self, int position, void* item);
void* linkedlist_remove_normal(struct LinkedList* self, int position);
void* linkedlist_get_normal(struct LinkedList* self, int position);
int linkedlist_indexof_normal(struct LinkedList* self, void* item);
int linkedlist_size_normal(struct LinkedList* self);

int linkedlist_add_concurrent(struct LinkedList* self, void* item);
int linkedlist_addto_concurrent(struct LinkedList* self, int position, void* item);
void* linkedlist_put_concurrent(struct LinkedList* self, int position, void* item);
void* linkedlist_remove_concurrent(struct LinkedList* self, int position);
void* linkedlist_get_concurrent(struct LinkedList* self, int position);
int linkedlist_indexof_concurrent(struct LinkedList* self, void* item);
int linkedlist_size_concurrent(struct LinkedList* self);

int linkedlistiterator_hasnext(struct LinkedListIterator* self);
void* linkedlistiterator_next(struct LinkedListIterator* self);

// local methods
struct LinkedItem* linkeditem_get(struct LinkedList* linkedlist, int position);

// implement methods
struct LinkedItem* linkeditem_get(struct LinkedList* linkedlist, int position) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)linkedlist;

    // check position is valid
    if (position < 0 || position > linkedlist_->size) {
        return NULL;
    }

    // broke iteration to two way for optimization
    struct LinkedItem* result = NULL;
    if (position <= linkedlist_->size / 2) {
        // iterate from foot to head
        int index = 0;
        result = linkedlist_->head;
        while (index < position) {
            result = result->next;
            index++;
        }
    } else {
        // iterate from head to foot
        int index = linkedlist_->size;
        result = linkedlist_->head->previews;
        while (index > position) {
            result = result->previews;
            index--;
        }
    }

    return result;
}

// normal mode vtable operators
int linkedlist_add_normal(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // normal addto
    int result = linkedlist_addto_normal(self, linkedlist_->size, item);

    return result;
}
int linkedlist_addto_normal(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position > linkedlist_->size) {
        return -1;
    }

    // get item before target item
    struct LinkedItem* item_target = linkeditem_get(self, position);

    // allocate new linkeditem and fill it
    struct LinkedItem* linkeditem = heap_alloc(sizeof(struct LinkedItem));
    linkeditem->item = item;
    linkeditem->next = item_target->next;
    linkeditem->previews = item_target;

    // add item to position
    int result = position;
    item_target->next->previews = linkeditem;
    item_target->next = linkeditem;
    linkedlist_->size++;

    return result;
}
void* linkedlist_put_normal(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // get target item
    struct LinkedItem* item_target = linkeditem_get(self, position + 1);

    // change item value
    void* result = item_target->item;
    item_target->item = item;

    return result;
}
void* linkedlist_remove_normal(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // get target item
    struct LinkedItem* item_target = linkeditem_get(self, position + 1);

    // change removable item next and previews
    item_target->next->previews = item_target->previews;
    item_target->previews->next = item_target->next;

    // remove item
    void* result = item_target->item;
    heap_free(item_target);
    linkedlist_->size--;

    return result;
}
void* linkedlist_get_normal(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // get target item
    struct LinkedItem* item_target = linkeditem_get(self, position + 1);

    // get item value
    void* result = item_target->item;

    return result;
}
int linkedlist_indexof_normal(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // search in items to find item
    int result = -1;
    struct LinkedItem* search_item = linkedlist_->head;
    do {
        // check comperator function is not NULL
        if (linkedlist_->comperator != NULL) {
            if (linkedlist_->comperator(item, search_item->item)) {
                break;
            }
        } else {
            if (item == search_item->item) {
                break;
            }
        }

        // go to next item
        result++;
        search_item = search_item->next;
    } while (search_item == linkedlist_->head);

    // check result is size -> not found
    if (result == linkedlist_->size) {
        result = -1;
    }

    return result;
}
int linkedlist_size_normal(struct LinkedList* self) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // get linkedlist size
    int result = linkedlist_->size;

    return result;
}

// concurrent mode vtable operators
int linkedlist_add_concurrent(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // concurrent writelock
    linkedlist_->rwlock->write_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal add
    int result = linkedlist_add_normal(self, item);

    // concurrent writeunlock
    linkedlist_->rwlock->write_unlock(linkedlist_->rwlock);

    return result;
}
int linkedlist_addto_concurrent(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position > linkedlist_->size) {
        return -1;
    }

    // concurrent writelock
    linkedlist_->rwlock->write_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal addto
    int result = linkedlist_addto_normal(self, position, item);

    // concurrent writeunlock
    linkedlist_->rwlock->write_unlock(linkedlist_->rwlock);

    return result;
}
void* linkedlist_put_concurrent(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // concurrent writelock
    linkedlist_->rwlock->write_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal put
    void* result = linkedlist_put_normal(self, position, item);

    // concurrent writeunlock
    linkedlist_->rwlock->write_unlock(linkedlist_->rwlock);

    return result;
}
void* linkedlist_remove_concurrent(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // concurrent writelock
    linkedlist_->rwlock->write_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal remove
    void* result = linkedlist_remove_normal(self, position);

    // concurrent writeunlock
    linkedlist_->rwlock->write_unlock(linkedlist_->rwlock);

    return result;
}
void* linkedlist_get_concurrent(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // concurrent writelock
    linkedlist_->rwlock->read_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal get
    void* result = linkedlist_get_normal(self, position);

    // concurrent writeunlock
    linkedlist_->rwlock->read_unlock(linkedlist_->rwlock);

    return result;
}
int linkedlist_indexof_concurrent(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // concurrent writelock
    linkedlist_->rwlock->read_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal indexof
    int result = linkedlist_indexof_normal(self, item);

    // concurrent writeunlock
    linkedlist_->rwlock->read_unlock(linkedlist_->rwlock);

    return result;
}
int linkedlist_size_concurrent(struct LinkedList* self) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)self;

    // concurrent writelock
    linkedlist_->rwlock->read_lock(linkedlist_->rwlock, UINT_64_MAX);

    // normal size
    int result = linkedlist_size_normal(self);

    // concurrent writeunlock
    linkedlist_->rwlock->read_unlock(linkedlist_->rwlock);

    return result;
}

int linkedlistiterator_hasnext(struct LinkedListIterator* self) {
    struct LinkedListIterator_* linkedlistiterator_ = (struct LinkedListIterator_*)self;

    // check end item is current item or not
    int result = linkedlistiterator_->end == linkedlistiterator_->item;

    return result;
}
void* linkedlistiterator_next(struct LinkedListIterator* self) {
    struct LinkedListIterator_* linkedlistiterator_ = (struct LinkedListIterator_*)self;

    // get current item value and move forward
    void* result = linkedlistiterator_->item;
    linkedlistiterator_->item = linkedlistiterator_->item->next;

    return result;
}

// object allocation and deallocation operators
void linkedlist_init() {
    // init normal vtable
    linkedlist_vtable_normal = heap_alloc(sizeof(LinkedList_VTable));
    linkedlist_vtable_normal->add = linkedlist_add_normal;
    linkedlist_vtable_normal->addto = linkedlist_addto_normal;
    linkedlist_vtable_normal->put = linkedlist_put_normal;
    linkedlist_vtable_normal->remove = linkedlist_remove_normal;
    linkedlist_vtable_normal->get = linkedlist_get_normal;
    linkedlist_vtable_normal->indexof = linkedlist_indexof_normal;
    linkedlist_vtable_normal->size = linkedlist_size_normal;

    // init concurrent vtable
    linkedlist_vtable_concurrent = heap_alloc(sizeof(LinkedList_VTable));
    linkedlist_vtable_concurrent->add = linkedlist_add_concurrent;
    linkedlist_vtable_concurrent->addto = linkedlist_addto_concurrent;
    linkedlist_vtable_concurrent->put = linkedlist_put_concurrent;
    linkedlist_vtable_concurrent->remove = linkedlist_remove_concurrent;
    linkedlist_vtable_concurrent->get = linkedlist_get_concurrent;
    linkedlist_vtable_concurrent->indexof = linkedlist_indexof_concurrent;
    linkedlist_vtable_concurrent->size = linkedlist_size_concurrent;
}
void linkedlistiterator_init() {
    // init vtable
    linkedlistiterator_vtable = heap_alloc(sizeof(LinkedListIterator_VTable));
    linkedlistiterator_vtable->hasnext = linkedlistiterator_hasnext;
    linkedlistiterator_vtable->next = linkedlistiterator_next;
}
LinkedList* linkedlist_new(int mode) {
    struct LinkedList_* linkedlist_ = heap_alloc(sizeof(struct LinkedList_));

    // set vtable
    switch (mode) {
        case 0:
            linkedlist_->self.vtable = linkedlist_vtable_normal;
            break;
        case 1:
            linkedlist_->self.vtable = linkedlist_vtable_concurrent;
            break;
    }

    // set constructor data
    linkedlist_->comperator = NULL;

    // set private data
    linkedlist_->size = 0;
    linkedlist_->head = NULL;
    linkedlist_->rwlock = NULL;

    return (LinkedList*)linkedlist_;
}
LinkedListIterator* linkedlistiterator_new() {
    struct LinkedListIterator_* linkedlistiterator_ = heap_alloc(sizeof(struct LinkedListIterator_));

    // set vtable
    linkedlistiterator_->self.vtable = linkedlistiterator_vtable;

    // set constructor data

    // set private data
    linkedlistiterator_->item = NULL;
    linkedlistiterator_->end = NULL;

    return (LinkedListIterator*)linkedlistiterator_;
}
void linkedlist_free(LinkedList* linkedlist) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)linkedlist;

    // free private data

    // iterate linkedlist and remove linkeditems
    linkedlist_->head->previews->next = NULL;
    struct LinkedItem* remove_item = NULL;
    do {
        remove_item = linkedlist_->head;
        linkedlist_->head = linkedlist_->head->next;
        heap_free(remove_item);
    } while (linkedlist_->head != NULL);

    if (linkedlist_->rwlock != NULL) {
        rwlock_free(linkedlist_->rwlock);
    }

    // free self
    heap_free(linkedlist_);
}
void linkedlistiterator_free(LinkedListIterator* linkedlistiterator) {
    struct LinkedListIterator_* linkedlistiterator_ = (struct LinkedList_*)linkedlistiterator;

    // free private data

    // free self
    heap_free(linkedlistiterator_);
}
LinkedList* linkedlist_new_object(int mode, int (*comperator)(void*, void*)) {
    struct LinkedList_* linkedlist_ = (struct LinkedList_*)linkedlist_new(mode);

    // set constructor data
    linkedlist_->comperator = comperator;

    // set private data
    linkedlist_->size = 0;

    // init head
    linkedlist_->head = heap_alloc(sizeof(struct LinkedItem));
    linkedlist_->head->next = linkedlist_->head;
    linkedlist_->head->previews = linkedlist_->head;
    linkedlist_->head->item = NULL;

    if (mode == 1) {
        linkedlist_->rwlock = rwlock_new(NULL);
    }

    return (LinkedList*)linkedlist_;
}
LinkedListIterator* linkedlistiterator_new_object(LinkedList* linkedlist) {
    struct LinkedListIterator_* linkedlistiterator_ = (struct LinkedListIterator_*)linkedlistiterator_new();

    struct LinkedList_* linkedlist_ = (struct LinkedList_*)linkedlist;

    // set constructor data

    // set private data
    linkedlistiterator_->item = linkedlist_->head->next;
    linkedlistiterator_->end = linkedlist_->head;

    return (LinkedListIterator*)linkedlistiterator_;
}