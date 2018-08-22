#include <low/dsa/ArrayList.h>

#include <io/memory/Memory.h>
#include <low/itc/high/RWLock.h>
#include <stdlib.h>

struct ArrayList_ {
    struct ArrayList self;
    int length;
    int cursor;
    void** array;
    float factor;
    struct RWLock* rwlock;
    int (*comperator)(void* item1, void* item2);
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
    struct ArrayList_* arraylist_ = self;

    // check array have free space or not
    if (arraylist_->cursor >= arraylist_->length) {
        arraylist_->length = arraylist_->length * arraylist_->factor;
        arraylist_->array = memory_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    // add item
    int result = arraylist_->cursor;
    arraylist_->array[arraylist_->cursor] = item;
    arraylist_->cursor++;

    return result;
}
int arraylist_addto_normal(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position > arraylist_->cursor) {
        return -1;
    }

    // check array have free space or not
    if (arraylist_->cursor >= arraylist_->length) {
        arraylist_->length = arraylist_->length * arraylist_->factor;
        arraylist_->array = memory_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    // move other items one block next
    memory_copy(
        (arraylist_->array + (position + 1 * sizeof(void*))),  // target address
        (arraylist_->array + ((position) * sizeof(void*))),    // source address
        (arraylist_->cursor - position) * sizeof(void*));      // size

    // add item
    int result = position;
    arraylist_->array[position] = item;
    arraylist_->cursor++;

    return result;
}
void* arraylist_put_normal(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = self;

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
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // get item at position
    void* result = arraylist_->array[position];

    // move other items on that item
    memory_copy(
        (arraylist_->array + (position * sizeof(void*))),        // target address
        (arraylist_->array + ((position + 1) * sizeof(void*))),  // source address
        (arraylist_->cursor - position) * sizeof(void*));        // size

    // remove item
    arraylist_->cursor--;

    // check array have free space or not
    if (arraylist_->cursor < arraylist_->length / arraylist_->factor) {
        arraylist_->length = arraylist_->length / arraylist_->factor;
        arraylist_->array = memory_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    return result;
}
void* arraylist_get_normal(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // get item at position
    void* result = arraylist_->array[position];

    return result;
}
int arraylist_indexof_normal(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = self;

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
    struct ArrayList_* arraylist_ = self;

    // get current cursor position (size)
    int result = arraylist_->cursor;

    return result;
}

int arraylist_add_concurrent(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = self;

    // concurrent writelock
    arraylist_->rwlock->writelock(arraylist_->rwlock);

    // normal add
    int result = arraylist_add_normal(self, item);

    // concurrent writeunlock
    arraylist_->rwlock->writeunlock(arraylist_->rwlock);

    return result;
}
int arraylist_addto_concurrent(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position > arraylist_->cursor) {
        return -1;
    }

    // concurrent writelock
    arraylist_->rwlock->writelock(arraylist_->rwlock);

    // normal addto
    int result = arraylist_addto_normal(self, position, item);

    // concurrent writeunlock
    arraylist_->rwlock->writeunlock(arraylist_->rwlock);

    return result;
}
void* arraylist_put_concurrent(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // concurrent writelock
    arraylist_->rwlock->writelock(arraylist_->rwlock);

    // normal put
    void* result = arraylist_put_normal(self, position, item);

    // concurrent writeunlock
    arraylist_->rwlock->writeunlock(arraylist_->rwlock);

    return result;
}
void* arraylist_remove_concurrent(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // concurrent writelock
    arraylist_->rwlock->writelock(arraylist_->rwlock);

    // normal remove
    void* result = arraylist_remove_normal(self, position);

    // concurrent writeunlock
    arraylist_->rwlock->writeunlock(arraylist_->rwlock);

    return result;
}
void* arraylist_get_concurrent(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // concurrent readlock
    arraylist_->rwlock->readlock(arraylist_->rwlock);

    // normal get
    void* result = arraylist_get_normal(self, position);

    // concurrent readunlock
    arraylist_->rwlock->readunlock(arraylist_->rwlock);

    return result;
}
int arraylist_indexof_concurrent(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = self;

    // concurrent readlock
    arraylist_->rwlock->readlock(arraylist_->rwlock);

    // normal indexof
    int result = arraylist_indexof_normal(self, item);

    // concurrent readunlock
    arraylist_->rwlock->readunlock(arraylist_->rwlock);

    return result;
}
int arraylist_size_concurrent(struct ArrayList* self) {
    struct ArrayList_* arraylist_ = self;

    // concurrent readlock
    arraylist_->rwlock->readlock(arraylist_->rwlock);

    // normal size
    int result = arraylist_size_normal(self);

    // concurrent readunlock
    arraylist_->rwlock->readunlock(arraylist_->rwlock);

    return result;
}

struct ArrayList* arraylist_new(int mode, float factor, int (*comperator)(void* item1, void* item2)) {
    struct ArrayList_* arraylist_ = memory_alloc(sizeof(struct ArrayList_));

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
            arraylist_->rwlock = rwlock_new();
            break;
    }

    // create internal cursor and length and array and factor
    arraylist_->length = 1;
    arraylist_->cursor = 0;
    arraylist_->array = memory_alloc(arraylist_->length * sizeof(void*));
    arraylist_->factor = factor;
    arraylist_->comperator = comperator;

    return arraylist_;
}
void arraylist_free(struct ArrayList* arraylist) {
    struct ArrayList_* arraylist_ = arraylist;

    // destry internal array and rwlock
    memory_free(arraylist_->array);
    if (arraylist_->rwlock != NULL) {
        rwlock_free(arraylist_->rwlock);
    }

    memory_free(arraylist_);
}