#include <low/Type.h>

// structs
typedef struct Condition Condition;
typedef struct Condition_VTable Condition_VTable;

// implement structs
// vtable + private data problem solve
struct Condition {
    Condition_VTable* vtable;
};

// vtable
struct Condition_VTable {
    int (*wait)(Condition* self, uint_64 timeout);
    int (*signal)(Condition* self, int count);
};

// init vtable
void condition_init();

// new raw condition
Condition* condition_new();

// free raw condition
void condition_free(Condition* condition);

// new condition
Condition* condition_new_object(char* name);