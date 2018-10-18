#include <high/Stack.h>

#include <high/Dequeue.h>
#include <low/Heap.h>

struct Stack_ {
    // self public object
    Stack self;

    // constructor data

    // private data
    Dequeue* dequeue;
};

// vtable
Stack_VTable* stack_vtable;

// link methods
int stack_push(Stack* self, uint_8* item, uint_64 timeout);
uint_8* stack_pop(Stack* self, uint_64 timeout);
uint_8* stack_get(Stack* self);
int stack_size(Stack* self);

// implement methods
// vtable operators
int stack_push(Stack* self, uint_8* item, uint_64 timeout) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // internal dequeue enqueue
    int result = stack_->dequeue->vtable->enqueue(stack_->dequeue, 0, item, timeout);

    return result;
}
uint_8* stack_pop(Stack* self, uint_64 timeout) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // internal dequeue dequeue
    uint_8* result = stack_->dequeue->vtable->dequeue(stack_->dequeue, 0, timeout);

    return result;
}
uint_8* stack_get(Stack* self) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // internal dequeue get
    uint_8* result = stack_->dequeue->vtable->get(stack_->dequeue, 0);

    return result;
}
int stack_size(Stack* self) {
    struct Stack_* stack_ = (struct Stack_*)self;

    // internal dequeue size
    int result = stack_->dequeue->vtable->size(stack_->dequeue);

    return result;
}

// object allocation and deallocation operators
void stack_init() {
    // init normal vtable
    stack_vtable = (Stack_VTable*) heap_alloc(sizeof(Stack_VTable));
    stack_vtable->push = stack_push;
    stack_vtable->pop = stack_pop;
    stack_vtable->get = stack_get;
    stack_vtable->size = stack_size;
}
Stack* stack_new(int mode) {
    struct Stack_* stack_ = (struct Stack_*) heap_alloc(sizeof(struct Stack_));

    // set vtable
    stack_->self.vtable = stack_vtable;

    // set constructor data

    // set private data
    stack_->dequeue = NULL;

    return (Stack*)stack_;
}
void stack_free(Stack* stack) {
    struct Stack_* stack_ = (struct Stack_*)stack;

    // free private data
    if (stack_->dequeue != NULL) {
        dequeue_free(stack_->dequeue);
    }

    // free self
    heap_free((uint_8*) stack_);
}
Stack* stack_new_object(int mode, int max, int (*comperator)(uint_8*, uint_8*)) {
    struct Stack_* stack_ = (struct Stack_*)stack_new(mode);

    // set constructor data

    // set private data
    stack_->dequeue = dequeue_new_object(mode, max, comperator);

    return (Stack*)stack_;
}
