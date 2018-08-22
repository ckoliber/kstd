typedef struct ArrayList {
    int (*add)(struct ArrayList* self, void* item);
    int (*addto)(struct ArrayList* self, int position, void* item);
    void* (*put)(struct ArrayList* self, int position, void* item);
    void* (*remove)(struct ArrayList* self, int position);
    void* (*get)(struct ArrayList* self, int position);
    int (*indexof)(struct ArrayList* self, void* item);
    int (*size)(struct ArrayList* self);
} ArrayList;

struct ArrayList* arraylist_new(int mode, float factor, int (*comperator)(void* item1, void* item2));
void arraylist_free(struct ArrayList* arraylist);