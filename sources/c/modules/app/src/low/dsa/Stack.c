#include <low/dsa/Stack.h>

#include <io/memory/Memory.h>
#include <low/dsa/Dequeue.h>

struct Stack_ {
    struct Stack self;
    struct Dequeue* dequeue;
};

// link methods
int stack_push(struct Stack* self, long int timeout, void* item);
void* stack_pop(struct Stack* self, long int timeout);
void* stack_get(struct Stack* self);
int stack_size(struct Stack* self);

int stack_push(struct Stack* self, long int timeout, void* item) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // Dequeue enqueue to front
    int result = stack_->dequeue->enqueue(stack_->dequeue, 1, item);

    return result;
}
void* stack_pop(struct Stack* self, long int timeout) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // Dequeue dequeue from front
    void* result = stack_->dequeue->dequeue(stack_->dequeue, 1, timeout);

    return result;
}
void* stack_get(struct Stack* self) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // Dequeue get from front
    void* result = stack_->dequeue->get(stack_->dequeue, 1);

    return result;
}
int stack_size(struct Stack* self) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // Dequeue get size
    int result = stack_->dequeue->size(stack_->dequeue);

    return result;
}

struct Stack* stack_new(int mode, int max, int (*comperator)(void*, void*)) {
    struct Stack_* stack_ = memory_alloc(sizeof(struct Stack_));

    // init private methods
    stack_->self.push = stack_push;
    stack_->self.pop = stack_pop;
    stack_->self.get = stack_get;
    stack_->self.size = stack_size;

    // init internal Dequeue
    stack_->dequeue = dequeue_new(mode, max, comperator);

    return (struct Stack*)stack_;
}
void stack_free(struct Stack* stack) {
    struct Stack_* stack_ = (struct Stack_*)stack;

    // destroy internal Dequeue
    dequeue_free(stack_->dequeue);

    memory_free(stack_);
}