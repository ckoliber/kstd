typedef struct LinkedList {
    int add(void* item);
    void* put(int position, void* item);
    void* remove(int position);
    void* get(int position);
    int indexof(void* item);
    int size();
} LinkedList;

struct LinkedList* linkedlist_new(int concurrent, int (*comperator)(void* item1, void* item2));
void linkedlist_free(struct LinkedList* linkedlist);