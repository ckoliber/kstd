#include <memory/low/Type.h>

typedef struct ArrayList {
    int (*add)(struct ArrayList* self, void* item);
    int (*addto)(struct ArrayList* self, int position, void* item);
    void* (*put)(struct ArrayList* self, int position, void* item);
    void* (*remove)(struct ArrayList* self, int position);
    void* (*get)(struct ArrayList* self, int position);
    int (*indexof)(struct ArrayList* self, void* item);
    int (*size)(struct ArrayList* self);
} ArrayList;

ArrayList* arraylist_new(int mode, float factor, int (*comperator)(void*, void*));
void arraylist_free(ArrayList* arraylist);