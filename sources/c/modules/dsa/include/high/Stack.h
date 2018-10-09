#include <low/Type.h>

// vtable
typedef struct Stack_VTable {
    int (*push)(struct Stack* self, void* item, uint_64 timeout);
    void* (*pop)(struct Stack* self, uint_64 timeout);
    void* (*get)(struct Stack* self);
    int (*size)(struct Stack* self);
} Stack_VTable;

// vtable + private data problem solve
typedef struct Stack {
    Stack_VTable* vtable;
} Stack;

// init vtable
void stack_init();

// new raw stack
Stack* stack_new(int mode);

// free raw stack
void stack_free(Stack* stack);

// new stack
Stack* stack_new_object(int mode, int max, int (*comperator)(void*, void*));