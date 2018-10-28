#include <low/Type.h>

// structs
typedef struct ArrayList ArrayList;
typedef struct ArrayList_VTable ArrayList_VTable;

// implement structs
// vtable + private data problem solve
struct ArrayList {
    ArrayList_VTable* vtable;
};

// vtable
struct ArrayList_VTable {
    int (*add)(ArrayList* self, void* item);
    int (*addto)(ArrayList* self, int position, void* item);
    void* (*put)(ArrayList* self, int position, void* item);
    void* (*remove)(ArrayList* self, int position);
    void* (*get)(ArrayList* self, int position);
    int (*indexof)(ArrayList* self, void* item);
    int (*size)(ArrayList* self);
};

// init vtable
void arraylist_init();

// new raw arraylist
ArrayList* arraylist_new(int mode);

// free raw arraylist
void arraylist_free(ArrayList* arraylist);

// new arraylist
ArrayList* arraylist_new_object(int mode, float factor, int (*comperator)(void*, void*));
