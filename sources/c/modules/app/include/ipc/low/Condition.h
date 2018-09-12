#include <memory/low/Type.h>

typedef struct Condition {
    int (*wait)(struct Condition* self, uint_64 timeout);
    int (*signal)(struct Condition* self, int count);
} Condition;

Condition* condition_new(char* name);
void condition_free(Condition* condition);