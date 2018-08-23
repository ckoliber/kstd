typedef struct Stack {
    int (*push)(struct Stack* self, void* item);
    void* (*pop)(struct Stack* self, int timeout);
    int (*size)(struct Stack* self);
} Stack;

struct Stack* stack_new(int mode, int (*comperator)(void*, void*));
void stack_free(struct Stack* stack);