#include <memory/low/Type.h>

typedef struct Condition {
    SignedInt wait(struct Condition* self, UnsignedLong timeout);
    SignedInt signal(struct Condition* self, UnsignedInt count);
} Condition;

Condition* condition_new(Char* name);
Void condition_free(Condition* condition);