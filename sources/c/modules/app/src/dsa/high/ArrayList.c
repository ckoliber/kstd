#include <dsa/high/ArrayList.h>

#include <ipc/high/RWLock.h>
#include <memory/low/Heap.h>

struct ArrayList_ {
    ArrayList self;
    int length;
    int cursor;
    void** array;
    float factor;
    RWLock* rwlock;
    int (*comperator)(void*, void*);
};

// link methods
int arraylist_add_normal(struct ArrayList* self, void* item);
int arraylist_addto_normal(struct ArrayList* self, int position, void* item);
void* arraylist_put_normal(struct ArrayList* self, int position, void* item);
void* arraylist_remove_normal(struct ArrayList* self, int position);
void* arraylist_get_normal(struct ArrayList* self, int position);
int arraylist_indexof_normal(struct ArrayList* self, void* item);
int arraylist_size_normal(struct ArrayList* self);

int arraylist_add_concurrent(struct ArrayList* self, void* item);
int arraylist_addto_concurrent(struct ArrayList* self, int position, void* item);
void* arraylist_put_concurrent(struct ArrayList* self, int position, void* item);
void* arraylist_remove_concurrent(struct ArrayList* self, int position);
void* arraylist_get_concurrent(struct ArrayList* self, int position);
int arraylist_indexof_concurrent(struct ArrayList* self, void* item);
int arraylist_size_concurrent(struct ArrayList* self);

int arraylist_add_normal(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // normal add
    int result = arraylist_addto_normal(self, arraylist_->cursor, item);

    return result;
}
int arraylist_addto_normal(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position > arraylist_->cursor) {
        return -1;
    }

    // check array have free space or not
    if (arraylist_->cursor >= arraylist_->length) {
        arraylist_->length = (int)(arraylist_->length * arraylist_->factor + 1);
        arraylist_->array = heap_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    // move other items one block next
    for (int cursor = arraylist_->cursor - 1; cursor >= position; cursor--) {
        arraylist_->array[cursor + 1] = arraylist_->array[cursor];
    }

    // add item
    int result = position;
    arraylist_->array[position] = item;
    arraylist_->cursor++;

    return result;
}
void* arraylist_put_normal(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // put item to position
    void* result = arraylist_->array[position];
    arraylist_->array[position] = item;

    return result;
}
void* arraylist_remove_normal(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // get item at position
    void* result = arraylist_->array[position];

    // move other items on that item
    for (int cursor = position + 1; cursor < arraylist_->cursor; cursor++) {
        arraylist_->array[cursor - 1] = arraylist_->array[cursor];
    }

    // remove item
    arraylist_->cursor--;

    // check array have free space or not
    if (arraylist_->cursor < arraylist_->length / arraylist_->factor) {
        arraylist_->length = (int)(arraylist_->length / arraylist_->factor);
        arraylist_->length = arraylist_->length ? arraylist_->length : 1;
        arraylist_->array = heap_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    return result;
}
void* arraylist_get_normal(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // get item at position
    void* result = arraylist_->array[position];

    return result;
}
int arraylist_indexof_normal(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // search in items to find item
    int result = -1;
    for (int cursor = 0; cursor < arraylist_->cursor; cursor++) {
        // check comperator function is not NULL
        if (arraylist_->comperator != NULL) {
            if (arraylist_->comperator(item, arraylist_->array[cursor])) {
                result = cursor;
                break;
            }
        } else {
            if (item == arraylist_->array[cursor]) {
                result = cursor;
                break;
            }
        }
    }

    return result;
}
int arraylist_size_normal(struct ArrayList* self) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // get current cursor position (size)
    int result = arraylist_->cursor;

    return result;
}

int arraylist_add_concurrent(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // concurrent writelock
    arraylist_->rwlock->write_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal add
    int result = arraylist_add_normal(self, item);

    // concurrent writeunlock
    arraylist_->rwlock->write_unlock(arraylist_->rwlock);

    return result;
}
int arraylist_addto_concurrent(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position > arraylist_->cursor) {
        return -1;
    }

    // concurrent writelock
    arraylist_->rwlock->write_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal addto
    int result = arraylist_addto_normal(self, position, item);

    // concurrent writeunlock
    arraylist_->rwlock->write_unlock(arraylist_->rwlock);

    return result;
}
void* arraylist_put_concurrent(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // concurrent writelock
    arraylist_->rwlock->write_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal put
    void* result = arraylist_put_normal(self, position, item);

    // concurrent writeunlock
    arraylist_->rwlock->write_unlock(arraylist_->rwlock);

    return result;
}
void* arraylist_remove_concurrent(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // concurrent writelock
    arraylist_->rwlock->write_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal remove
    void* result = arraylist_remove_normal(self, position);

    // concurrent writeunlock
    arraylist_->rwlock->write_unlock(arraylist_->rwlock);

    return result;
}
void* arraylist_get_concurrent(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // concurrent readlock
    arraylist_->rwlock->read_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal get
    void* result = arraylist_get_normal(self, position);

    // concurrent readunlock
    arraylist_->rwlock->read_unlock(arraylist_->rwlock);

    return result;
}
int arraylist_indexof_concurrent(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // concurrent readlock
    arraylist_->rwlock->read_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal indexof
    int result = arraylist_indexof_normal(self, item);

    // concurrent readunlock
    arraylist_->rwlock->read_unlock(arraylist_->rwlock);

    return result;
}
int arraylist_size_concurrent(struct ArrayList* self) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)self;

    // concurrent readlock
    arraylist_->rwlock->read_lock(arraylist_->rwlock, UINT_64_MAX);

    // normal size
    int result = arraylist_size_normal(self);

    // concurrent readunlock
    arraylist_->rwlock->read_unlock(arraylist_->rwlock);

    return result;
}

ArrayList* arraylist_new(int mode, float factor, int (*comperator)(void*, void*)) {
    struct ArrayList_* arraylist_ = heap_alloc(sizeof(struct ArrayList_));

    // init private methods
    switch (mode) {
        case 0:
            arraylist_->self.add = arraylist_add_normal;
            arraylist_->self.addto = arraylist_addto_normal;
            arraylist_->self.put = arraylist_put_normal;
            arraylist_->self.remove = arraylist_remove_normal;
            arraylist_->self.get = arraylist_get_normal;
            arraylist_->self.indexof = arraylist_indexof_normal;
            arraylist_->self.size = arraylist_size_normal;
            arraylist_->rwlock = NULL;
            break;
        case 1:
            arraylist_->self.add = arraylist_add_concurrent;
            arraylist_->self.addto = arraylist_addto_concurrent;
            arraylist_->self.put = arraylist_put_concurrent;
            arraylist_->self.remove = arraylist_remove_concurrent;
            arraylist_->self.get = arraylist_get_concurrent;
            arraylist_->self.indexof = arraylist_indexof_concurrent;
            arraylist_->self.size = arraylist_size_concurrent;
            arraylist_->rwlock = rwlock_new(NULL);
            break;
    }

    // create internal cursor and length and array and factor
    arraylist_->length = 1;
    arraylist_->cursor = 0;
    arraylist_->array = heap_alloc(arraylist_->length * sizeof(void*));
    arraylist_->factor = factor;
    arraylist_->comperator = comperator;

    return (ArrayList*)arraylist_;
}
void arraylist_free(ArrayList* arraylist) {
    struct ArrayList_* arraylist_ = (struct ArrayList_*)arraylist;

    // destry internal array and rwlock
    heap_free(arraylist_->array);
    if (arraylist_->rwlock != NULL) {
        rwlock_free(arraylist_->rwlock);
    }

    heap_free(arraylist_);
}