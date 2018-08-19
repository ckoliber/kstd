typedef struct Stack {
    int push(void* item);
    void* pop();
    void* blockpop();
    void* tryblockpop(int timeout);
    int size();
} Stack;

struct Stack* stack_new(int concurrent, int (*comperator)(void* item1, void* item2));
void stack_free(struct Stack* stack);