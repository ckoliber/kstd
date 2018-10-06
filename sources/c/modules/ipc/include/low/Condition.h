#include <memory/low/Type.h>

// vtable
typedef struct Condition_VTable {
    int (*wait)(struct Condition* self, uint_64 timeout);
    int (*signal)(struct Condition* self, int count);
} Condition_VTable;

// vtable + private data problem solve
typedef struct Condition {
    Condition_VTable* vtable;
} Condition;

// init vtable
void condition_init();

// new raw condition
Condition* condition_new();

// free raw condition
void condition_free(Condition* condition);

// new condition
Condition* condition_new_object(char* name);