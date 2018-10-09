#include <low/Type.h>

// vtable
typedef struct LinkedList_VTable {
    int (*add)(struct LinkedList* self, void* item);
    int (*addto)(struct LinkedList* self, int position, void* item);
    void* (*put)(struct LinkedList* self, int position, void* item);
    void* (*remove)(struct LinkedList* self, int position);
    void* (*get)(struct LinkedList* self, int position);
    int (*indexof)(struct LinkedList* self, void* item);
    int (*size)(struct LinkedList* self);
} LinkedList_VTable;

typedef struct LinkedListIterator_VTable {
    bool (*hasnext)(struct LinkedListIterator* self);
    void* (*next)(struct LinkedListIterator* self);
} LinkedListIterator_VTable;

// vtable + private data problem solve
typedef struct LinkedList {
    LinkedList_VTable* vtable;
} LinkedList;

typedef struct LinkedListIterator {
    LinkedListIterator_VTable* vtable;
} LinkedListIterator;

// init vtable
void linkedlist_init();
void linkedlistiterator_init();

// new raw linkedlist
LinkedList* linkedlist_new(int mode);
LinkedListIterator* linkedlistiterator_new();

// free raw linkedlist
void linkedlist_free(LinkedList* linkedlist);
void linkedlistiterator_free(LinkedListIterator* linkedlistiterator);

// new linkedlist
LinkedList* linkedlist_new_object(int mode, int (*comperator)(void*, void*));
LinkedListIterator* linkedlistiterator_new_object(LinkedList* linkedlist);