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
int arraylist_add(struct ArrayList* self, void* item);
int arraylist_addto(struct ArrayList* self, int position, void* item);
void* arraylist_put(struct ArrayList* self, int position, void* item);
void* arraylist_remove(struct ArrayList* self, int position);
void* arraylist_get(struct ArrayList* self, int position);
int arraylist_indexof(struct ArrayList* self, void* item);
int arraylist_size(struct ArrayList* self);

int arraylist_add(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check array is concurrent then writelock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writelock(arraylist_->rwlock);
    }

    // check array have free space or not
    if (arraylist_->cursor >= arraylist_->length) {
        arraylist_->length = arraylist_->length * arraylist_->factor;
        arraylist_->array = memory_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    // add item
    int result = arraylist_->cursor;
    arraylist_->array[arraylist_->cursor] = item;
    arraylist_->cursor++;

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writeunlock(arraylist_->rwlock);
    }

    return result;
}
int arraylist_addto(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check array is concurrent then writelock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writelock(arraylist_->rwlock);
    }

    // check array have free space or not
    if (arraylist_->cursor >= arraylist_->length) {
        arraylist_->length = arraylist_->length * arraylist_->factor;
        arraylist_->array = memory_realloc(arraylist_->array, arraylist_->length * sizeof(void*));
    }

    // move other items one block next
    memory_copy(
        (arraylist_->array + (position + 1 * sizeof(void*))),  // remove item address
        (arraylist_->array + ((position) * sizeof(void*))),    // other items address
        (arraylist_->cursor - position));                      // count

    // add item
    int result = position;
    arraylist_->array[position] = item;
    arraylist_->cursor++;

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writeunlock(arraylist_->rwlock);
    }

    return result;
}
void* arraylist_put(struct ArrayList* self, int position, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // check array is concurrent then writelock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writelock(arraylist_->rwlock);
    }

    // put item to position
    void* result = arraylist_->array[position];
    arraylist_->array[position] = item;

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writeunlock(arraylist_->rwlock);
    }

    return result;
}
void* arraylist_remove(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // check array is concurrent then writelock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writelock(arraylist_->rwlock);
    }

    // get item at position
    void* result = arraylist_->array[position];

    // move other items on that item
    memory_copy(
        (arraylist_->array + (position * sizeof(void*))),        // remove item address
        (arraylist_->array + ((position + 1) * sizeof(void*))),  // other items address
        (arraylist_->cursor - position));                        // count

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->writeunlock(arraylist_->rwlock);
    }

    return result;
}
void* arraylist_get(struct ArrayList* self, int position) {
    struct ArrayList_* arraylist_ = self;

    // check position is valid
    if (position < 0 || position >= arraylist_->cursor) {
        return NULL;
    }

    // check array is concurrent then readlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->readlock(arraylist_->rwlock);
    }

    // get item at position
    void* result = arraylist_->array[position];

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->readunlock(arraylist_->rwlock);
    }

    return result;
}
int arraylist_indexof(struct ArrayList* self, void* item) {
    struct ArrayList_* arraylist_ = self;

    // check array is concurrent then readlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->readlock(arraylist_->rwlock);
    }

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

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->readunlock(arraylist_->rwlock);
    }

    return result;
}
int arraylist_size(struct ArrayList* self) {
    struct ArrayList_* arraylist_ = self;

    // check array is concurrent then readlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->readlock(arraylist_->rwlock);
    }

    // get current cursor position (size)
    int result = arraylist_->cursor;

    // check array is concurrent then writeunlock
    if (arraylist_->rwlock != NULL) {
        arraylist_->rwlock->readunlock(arraylist_->rwlock);
    }

    return result;
}

struct ArrayList* arraylist_new(int concurrent, float factor, int (*comperator)(void* item1, void* item2)) {
    struct ArrayList_* arraylist_ = memory_alloc(sizeof(struct ArrayList_));

    // init private methods
    arraylist_->self.add = arraylist_add;
    arraylist_->self.addto = arraylist_addto;
    arraylist_->self.put = arraylist_put;
    arraylist_->self.remove = arraylist_remove;
    arraylist_->self.get = arraylist_get;
    arraylist_->self.indexof = arraylist_indexof;
    arraylist_->self.size = arraylist_size;

    // create internal cursor and length and array and factor and rwlock
    arraylist_->length = 1;
    arraylist_->cursor = 0;
    arraylist_->array = memory_alloc(arraylist_->length * sizeof(void*));
    arraylist_->factor = factor;
    arraylist_->comperator = comperator;
    if (concurrent) {
        arraylist_->rwlock = rwlock_new();
    } else {
        arraylist_->rwlock = NULL;
    }

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