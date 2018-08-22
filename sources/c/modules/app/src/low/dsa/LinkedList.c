#include <low/dsa/LinkedList.h>

#include <io/memory/Memory.h>
#include <low/itc/high/RWLock.h>
#include <stdlib.h>

struct LinkedList_ {
    struct LinkedList self;
    struct LinkedItem* head;
    int size;
    struct RWLock* rwlock;
    int (*comperator)(void* item1, void* item2);
};

struct LinkedItem {
    struct LinkedItem* next;
    struct LinkedItem* previews;
    void* item;
};

struct LinkedListIterator_ {
    struct LinkedListIterator self;
    struct LinkedItem* item;
    struct LinkedItem* end;
};

// link methods
int linkedlist_add(struct LinkedList* self, void* item);
int linkedlist_addto(struct LinkedList* self, int position, void* item);
void* linkedlist_put(struct LinkedList* self, int position, void* item);
void* linkedlist_remove(struct LinkedList* self, int position);
void* linkedlist_get(struct LinkedList* self, int position);
int linkedlist_indexof(struct LinkedList* self, void* item);
int linkedlist_size(struct LinkedList* self);

// iterator linked methods
int linkedlistiterator_hasnext(struct LinkedListIterator* self);
void* linkedlistiterator_next(struct LinkedListIterator* self);

// local methods
struct LinkedItem* item_get(struct LinkedList* linkedlist, int position);

struct LinkedItem* item_get(struct LinkedList* linkedlist, int position) {
    struct LinkedList_* linkedlist_ = linkedlist;

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

int linkedlist_add(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = self;

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writelock(linkedlist_->rwlock);
    }

    // get target add position item
    struct LinkedItem* item_target = linkedlist_->head->previews;

    // allocate new linkeditem and fill it
    struct LinkedItem* linkeditem = memory_alloc(sizeof(struct LinkedItem));
    linkeditem->item = item;
    linkeditem->next = item_target->next;
    linkeditem->previews = item_target;

    // add item to position
    int result = linkedlist_->size;
    item_target->next->previews = linkeditem;
    item_target->next = linkeditem;
    linkedlist_->size++;

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writeunlock(linkedlist_->rwlock);
    }

    return result;
}
int linkedlist_addto(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = self;

    // check position is valid
    if (position < 0 || position > linkedlist_->size) {
        return -1;
    }

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writelock(linkedlist_->rwlock);
    }

    // get target item
    struct LinkedItem* item_target = item_get(position);

    // allocate new linkeditem and fill it
    struct LinkedItem* linkeditem = memory_alloc(sizeof(struct LinkedItem));
    linkeditem->item = item;
    linkeditem->next = item_target->next;
    linkeditem->previews = item_target;

    // add item to position
    int result = position;
    item_target->next->previews = linkeditem;
    item_target->next = linkeditem;
    linkedlist_->size++;

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writeunlock(linkedlist_->rwlock);
    }

    return result;
}
void* linkedlist_put(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writelock(linkedlist_->rwlock);
    }

    // get target item
    struct LinkedItem* item_target = item_get(position + 1);

    // change item value
    void* result = item_target->item;
    item_target->item = item;

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writeunlock(linkedlist_->rwlock);
    }

    return result;
}
void* linkedlist_remove(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writelock(linkedlist_->rwlock);
    }

    // get target item
    struct LinkedItem* item_target = item_get(position + 1);

    // change removable item next and previews
    item_target->next->previews = item_target->previews;
    item_target->previews->next = item_target->next;

    // remove item
    void* result = item_target->item;
    memory_free(item_target);

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writeunlock(linkedlist_->rwlock);
    }

    return result;
}
void* linkedlist_get(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = self;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->readlock(linkedlist_->rwlock);
    }

    // get target item
    struct LinkedItem* item_target = item_get(position + 1);

    // get item value
    void* result = item_target->item;

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->readunlock(linkedlist_->rwlock);
    }

    return result;
}
int linkedlist_indexof(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = self;

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->readlock(linkedlist_->rwlock);
    }

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

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->readunlock(linkedlist_->rwlock);
    }

    return result;
}
int linkedlist_size(struct LinkedList* self) {
    struct LinkedList_* linkedlist_ = self;

    // check array is concurrent then writelock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->readlock(linkedlist_->rwlock);
    }

    // get linkedlist size
    int result = linkedlist_->size;

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->readunlock(linkedlist_->rwlock);
    }

    return result;
}

int linkedlistiterator_hasnext(struct LinkedListIterator* self) {
    struct LinkedListIterator_* linkedlistiterator_ = self;

    // check end item is current item or not
    int result = linkedlistiterator_->end == linkedlistiterator_->item;

    return result;
}
void* linkedlistiterator_next(struct LinkedListIterator* self) {
    struct LinkedListIterator_* linkedlistiterator_ = self;

    // get current item value and move forward
    void* result = linkedlistiterator_->item;
    linkedlistiterator_->item = linkedlistiterator_->item->next;

    return result;
}

struct LinkedList* linkedlist_new(int concurrent, int (*comperator)(void* item1, void* item2)) {
    struct LinkedList_* linkedlist_ = memory_alloc(sizeof(struct LinkedList_));

    // init private methods
    linkedlist_->self.add = linkedlist_add;
    linkedlist_->self.addto = linkedlist_addto;
    linkedlist_->self.put = linkedlist_put;
    linkedlist_->self.remove = linkedlist_remove;
    linkedlist_->self.get = linkedlist_get;
    linkedlist_->self.indexof = linkedlist_indexof;
    linkedlist_->self.size = linkedlist_size;

    // init size and head
    linkedlist_->size = 0;
    linkedlist_->head = memory_alloc(sizeof(struct LinkedItem));
    linkedlist_->head->next = linkedlist_->head;
    linkedlist_->head->previews = linkedlist_->head;
    linkedlist_->head->item = NULL;

    // init comperator and rwlock
    linkedlist_->comperator = comperator;
    if (concurrent) {
        linkedlist_->rwlock = rwlock_new();
    } else {
        linkedlist_->rwlock = NULL;
    }

    return linkedlist_;
}
void linkedlist_free(struct LinkedList* linkedlist) {
    struct LinkedList_* linkedlist_ = linkedlist;

    // break linkedlist circle
    linkedlist_->head->previews->next = NULL;

    // iterate linkedlist and remove linkeditems
    struct LinkedItem* remove_item = NULL;
    do {
        remove_item = linkedlist_->head;
        linkedlist_->head = linkedlist_->head->next;
        memory_free(remove_item);
    } while (remove_item != NULL);

    // destry internal rwlock
    if (linkedlist_->rwlock != NULL) {
        rwlock_free(linkedlist_->rwlock);
    }

    memory_free(linkedlist_);
}

struct LinkedListIterator* linkedlistiterator_new(struct LinkedList* linkedlist) {
    struct LinkedListIterator_* linkedlistiterator_ = memory_alloc(sizeof(struct LinkedListIterator_));

    // init private methods
    linkedlistiterator_->self.hasnext = linkedlistiterator_hasnext;
    linkedlistiterator_->self.next = linkedlistiterator_next;

    // init size and head
    struct LinkedList_* linkedlist_ = linkedlist;
    linkedlistiterator_->item = linkedlist_->head->next;
    linkedlistiterator_->end = linkedlist_->head;

    return linkedlistiterator_;
}
void linkedlistiterator_free(struct LinkedListIterator* linkedlistiterator) {
    struct LinkedListIterator_* linkedlistiterator_ = linkedlistiterator;

    memory_free(linkedlistiterator_);
}