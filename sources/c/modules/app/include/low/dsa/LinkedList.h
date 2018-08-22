typedef struct LinkedList {
    int (*add)(struct LinkedList* self, void* item);
    int (*addto)(struct LinkedList* self, int position, void* item);
    void* (*put)(struct LinkedList* self, int position, void* item);
    void* (*remove)(struct LinkedList* self, int position);
    void* (*get)(struct LinkedList* self, int position);
    int (*indexof)(struct LinkedList* self, void* item);
    int (*size)(struct LinkedList* self);
} LinkedList;

typedef struct LinkedListIterator {
    int (*hasnext)(struct LinkedListIterator* self);
    void* (*next)(struct LinkedListIterator* self);
} LinkedListIterator;

struct LinkedList* linkedlist_new(int concurrent, int (*comperator)(void* item1, void* item2));
void linkedlist_free(struct LinkedList* linkedlist);

struct LinkedListIterator* linkedlistiterator_new(struct LinkedList* linkedlist);
void linkedlistiterator_free(struct LinkedListIterator* linkedlistiterator);