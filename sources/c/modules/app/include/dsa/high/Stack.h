typedef struct Stack {
    int (*push)(struct Stack* self, long int timeout, void* item);
    void* (*pop)(struct Stack* self, long int timeout);
    void* (*get)(struct Stack* self);
    int (*size)(struct Stack* self);
} Stack;

struct Stack* stack_new(int mode, int max, int (*comperator)(void*, void*));
void stack_free(struct Stack* stack);