typedef struct Stack {
    int (*push)(struct Stack* self, void* item);
    void* (*pop)(struct Stack* self);
    void* (*blockpop)(struct Stack* self);
    void* (*timeblockpop)(struct Stack* self, int timeout);
    int (*size)(struct Stack* self);
} Stack;

struct Stack* stack_new(int concurrent, int (*comperator)(void* item1, void* item2));
void stack_free(struct Stack* stack);