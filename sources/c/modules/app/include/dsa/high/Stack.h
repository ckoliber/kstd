#include <memory/low/Type.h>

typedef struct Stack {
    int (*push)(struct Stack* self, void* item, uint_64 timeout);
    void* (*pop)(struct Stack* self, uint_64 timeout);
    void* (*get)(struct Stack* self);
    int (*size)(struct Stack* self);
} Stack;

Stack* stack_new(int mode, int max, int (*comperator)(void*, void*));
void stack_free(Stack* stack);