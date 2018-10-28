#include <low/Type.h>

// structs
typedef struct Stack Stack;
typedef struct Stack_VTable Stack_VTable;

// implement structs
// vtable + private data problem solve
struct Stack {
    Stack_VTable* vtable;
};

// vtable
struct Stack_VTable {
    int (*push)(Stack* self, void* item, uint_64 timeout);
    void* (*pop)(Stack* self, uint_64 timeout);
    void* (*get)(Stack* self);
    int (*size)(Stack* self);
};

// init vtable
void stack_init();

// new raw stack
Stack* stack_new(int mode);

// free raw stack
void stack_free(Stack* stack);

// new stack
Stack* stack_new_object(int mode, int max, int (*comperator)(void*, void*));
