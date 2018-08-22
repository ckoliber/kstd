#include <low/dsa/LinkedList.h>

#include <io/memory/Memory.h>
#include <low/itc/high/RWLock.h>
#include <stdlib.h>

struct LinkedList_ {
    struct LinkedList self;
    struct LinkedItem* head;
    struct LinkedItem* foot;
    int size;
    struct RWLock* rwlock;
    int (*comperator)(void* item1, void* item2);
};

struct LinkedItem {
    struct LinkedItem* next;
    struct LinkedItem* previews;
    void* item;
};

// link methods
int linkedlist_add(struct LinkedList* self, void* item);
int linkedlist_addto(struct LinkedList* self, int position, void* item);
void* linkedlist_put(struct LinkedList* self, int position, void* item);
void* linkedlist_remove(struct LinkedList* self, int position);
void* linkedlist_get(struct LinkedList* self, int position);
int linkedlist_indexof(struct LinkedList* self, void* item);
int linkedlist_size(struct LinkedList* self);

// local methods
struct LinkedItem* item_get(struct LinkedList* linkedlist, int position);

struct LinkedItem* item_get(struct LinkedList* linkedlist, int position) {
    struct LinkedList_* linkedlist_ = linkedlist;

    // check position is valid
    if (position < 0 || position >= linkedlist_->size) {
        return NULL;
    }

    // broke iteration to two way for optimization
    struct LinkedItem* result = NULL;
    if (position <= linkedlist_->size / 2) {
        // iterate from foot to head
        int index = 0;
        result = linkedlist_->foot;
        while (index < position) {
            result = result->next;
            index++;
        }
    } else {
        // iterate from head to foot
        int index = linkedlist_->size - 1;
        result = linkedlist_->head;
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

    // allocate new linkeditem and fill it
    struct LinkedItem* linkeditem = memory_alloc(sizeof(struct LinkedItem));
    linkeditem->item = item;

    // add linkeditem to head item
    int result = linkedlist_->size;
    if (linkedlist_->size > 0) {
        // init linkeditem
        linkeditem->next = NULL;
        linkeditem->previews = linkedlist_->head;

        // add linkeditem
        linkedlist_->head->next = linkeditem;
        linkedlist_->head = linkeditem;
    } else {
        // init linkeditem
        linkeditem->next = NULL;
        linkeditem->previews = linkedlist_->head;

        // add linkeditem
        linkedlist_->head = linkeditem;
        linkedlist_->foot = linkeditem;
    }
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

    // allocate new linkeditem and fill it
    struct LinkedItem* linkeditem = memory_alloc(sizeof(struct LinkedItem));
    linkeditem->item = item;

    // add linkeditem to head item
    int result = linkedlist_->size;
    if (linkedlist_->size > 0) {
        // get target item
        struct LinkedItem* item_target = item_get(linkedlist_, position);

        // init linkeditem
        linkeditem->next = item_target;
        linkeditem->previews = item_target->previews;

        // add linkeditem
        linkedlist_->head->next = linkeditem;
        linkedlist_->head = linkeditem;
    } else {
        // init linkeditem
        linkeditem->next = NULL;
        linkeditem->previews = linkedlist_->head;

        // add linkeditem
        linkedlist_->head = linkeditem;
        linkedlist_->foot = linkeditem;
    }
    linkedlist_->size++;

    // check array is concurrent then writeunlock
    if (linkedlist_->rwlock != NULL) {
        linkedlist_->rwlock->writeunlock(linkedlist_->rwlock);
    }

    return 0;
}
void* linkedlist_put(struct LinkedList* self, int position, void* item) {
    struct LinkedList_* linkedlist_ = self;

    return 0;
}
void* linkedlist_remove(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = self;

    return 0;
}
void* linkedlist_get(struct LinkedList* self, int position) {
    struct LinkedList_* linkedlist_ = self;

    return 0;
}
int linkedlist_indexof(struct LinkedList* self, void* item) {
    struct LinkedList_* linkedlist_ = self;

    return 0;
}
int linkedlist_size(struct LinkedList* self) {
    struct LinkedList_* linkedlist_ = self;

    return 0;
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

    // create internal head and foot and size and rwlock
    linkedlist_->head = NULL;
    linkedlist_->foot = NULL;
    linkedlist_->size = 0;
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

    // destry internal linkeditems and rwlock
    // iterate linked and free them
    if (linkedlist_->rwlock != NULL) {
        rwlock_free(linkedlist_->rwlock);
    }

    memory_free(linkedlist_);
}