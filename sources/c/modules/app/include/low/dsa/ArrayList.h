typedef struct ArrayList {
    int add(void* item);
    void* put(int position, void* item);
    void* remove(int position);
    void* get(int position);
    int indexof(void* item);
    int size();
} ArrayList;

struct ArrayList* arraylist_new(int concurrent, float factor, int (*comperator)(void* item1, void* item2));
void arraylist_free(struct ArrayList* arraylist);