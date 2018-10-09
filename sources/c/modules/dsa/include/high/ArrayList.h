#include <low/Type.h>

// vtable
typedef struct ArrayList_VTable {
    int (*add)(struct ArrayList* self, void* item);
    int (*addto)(struct ArrayList* self, int position, void* item);
    void* (*put)(struct ArrayList* self, int position, void* item);
    void* (*remove)(struct ArrayList* self, int position);
    void* (*get)(struct ArrayList* self, int position);
    int (*indexof)(struct ArrayList* self, void* item);
    int (*size)(struct ArrayList* self);
} ArrayList_VTable;

// vtable + private data problem solve
typedef struct ArrayList {
    ArrayList_VTable* vtable;
} ArrayList;

// init vtable
void arraylist_init();

// new raw arraylist
ArrayList* arraylist_new(int mode);

// free raw arraylist
void arraylist_free(ArrayList* arraylist);

// new arraylist
ArrayList* arraylist_new_object(int mode, float factor, int (*comperator)(void*, void*));