#include <low/Type.h>

// structs
typedef struct LinkedList LinkedList;
typedef struct LinkedListIterator LinkedListIterator;
typedef struct LinkedList_VTable LinkedList_VTable;
typedef struct LinkedListIterator_VTable LinkedListIterator_VTable;

// implement structs
// vtable + private data problem solve
struct LinkedList {
    LinkedList_VTable* vtable;
};

struct LinkedListIterator {
    LinkedListIterator_VTable* vtable;
};

// vtable
struct LinkedList_VTable {
    int (*add)(LinkedList* self, uint_8* item);
    int (*addto)(LinkedList* self, int position, uint_8* item);
    uint_8* (*put)(LinkedList* self, int position, uint_8* item);
    uint_8* (*remove)(LinkedList* self, int position);
    uint_8* (*get)(LinkedList* self, int position);
    int (*indexof)(LinkedList* self, uint_8* item);
    int (*size)(LinkedList* self);
};

struct LinkedListIterator_VTable {
    bool (*hasnext)(LinkedListIterator* self);
    uint_8* (*next)(LinkedListIterator* self);
};

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
LinkedList* linkedlist_new_object(int mode, int (*comperator)(uint_8*, uint_8*));
LinkedListIterator* linkedlistiterator_new_object(LinkedList* linkedlist);
