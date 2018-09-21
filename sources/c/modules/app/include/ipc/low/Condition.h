#include <memory/low/Type.h>

typedef struct Condition {
    int (*wait)(struct Condition* self, uint_64 timeout);
    int (*signal)(struct Condition* self, int count);
} Condition;

// init vtable
void condition_init();

// new raw condition
Condition* condition_new();

// free raw condition
void condition_free(Condition* condition);

// new condition
Condition* condition_new_object(char* name);