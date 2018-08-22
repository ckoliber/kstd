#include <low/dsa/Stack.h>

#include <io/memory/Memory.h>
#include <low/itc/high/RWLock.h>
#include <low/itc/high/Semaphore.h>
#include <stdlib.h>

struct Stack_ {
    struct Stack self;
    struct StackItem* head;
    int size;
    struct RWLock* rwlock;
    struct Semaphore* semaphore;
    int (*comperator)(void* item1, void* item2);
};

struct StackItem {
    struct StackItem* next;
    void* item;
};

// link methods
int stack_push_normal(struct Stack* self, void* item);
void* stack_pop_normal(struct Stack* self, int timeout);
int stack_size_normal(struct Stack* self);

int stack_push_concurrent(struct Stack* self, void* item);
void* stack_pop_concurrent(struct Stack* self, int timeout);
int stack_size_concurrent(struct Stack* self);

int stack_push_blocking(struct Stack* self, void* item);
void* stack_pop_blocking(struct Stack* self, int timeout);
int stack_size_blocking(struct Stack* self);

int stack_push_normal(struct Stack* self, void* item) {
    struct Stack_* stack_ = self;

    // allocate new stackitem and fill it
    struct StackItem* stackitem = memory_alloc(sizeof(struct StackItem));
    stackitem->item = item;
    stackitem->next = stack_->head->next;

    // add item to position
    int result = stack_->size;
    stack_->head->next = stackitem;
    stack_->size++;

    return result;
}
void* stack_pop_normal(struct Stack* self, int timeout) {
    struct Stack_* stack_ = self;

    // check stack_ is not empty
    if (stack_->size <= 0) {
        return NULL;
    }

    // change removable item next
    struct StackItem* item_target = stack_->head->next;
    stack_->head->next = stack_->head->next->next;

    // remove item
    void* result = item_target->item;
    memory_free(item_target);
    stack_->size--;

    return result;
}
int stack_size_normal(struct Stack* self) {
    struct Stack_* stack_ = self;

    // get stack size
    int result = stack_->size;

    return result;
}

int stack_push_concurrent(struct Stack* self, void* item) {
    struct Stack_* stack_ = self;

    // concurrent writelock
    stack_->rwlock->writelock(stack_->rwlock);

    // normal push
    int result = stack_push_normal(self, item);

    // concurrent writeunlock
    stack_->rwlock->writeunlock(stack_->rwlock);

    return result;
}
void* stack_pop_concurrent(struct Stack* self, int timeout) {
    struct Stack_* stack_ = self;

    // concurrent writelock
    stack_->rwlock->writelock(stack_->rwlock);

    // normal pop
    void* result = stack_pop_normal(self, timeout);

    // concurrent writeunlock
    stack_->rwlock->writeunlock(stack_->rwlock);

    return result;
}
int stack_size_concurrent(struct Stack* self) {
    struct Stack_* stack_ = self;

    // concurrent readlock
    stack_->rwlock->readlock(stack_->rwlock);

    // normal size
    int result = stack_size_normal(self);

    // concurrent readunlock
    stack_->rwlock->readunlock(stack_->rwlock);

    return result;
}

int stack_push_blocking(struct Stack* self, void* item) {
    struct Stack_* stack_ = self;

    // normal push
    int result = stack_push_normal(self, item);

    // signal on semaphore
    stack_->semaphore->post(stack_->semaphore, 1);

    return result;
}
void* stack_pop_blocking(struct Stack* self, int timeout) {
    struct Stack_* stack_ = self;

    // wait on semaphore
    stack_->semaphore->timewait(stack_->semaphore, 1, timeout);

    // normal pop
    int result = stack_pop_normal(self, timeout);

    return result;
}
int stack_size_blocking(struct Stack* self) {
    struct Stack_* stack_ = self;

    // normal size
    int result = stack_size_normal(self);

    return result;
}

struct Stack* stack_new(int mode, int (*comperator)(void* item1, void* item2)) {
    struct Stack_* stack_ = memory_alloc(sizeof(struct Stack_));

    // init private methods
    switch (mode) {
        case 0:
            stack_->self.push = stack_push_normal;
            stack_->self.pop = stack_pop_normal;
            stack_->self.size = stack_size_normal;
            stack_->rwlock = NULL;
            stack_->semaphore = NULL;
            break;
        case 1:
            stack_->self.push = stack_push_concurrent;
            stack_->self.pop = stack_pop_concurrent;
            stack_->self.size = stack_size_concurrent;
            stack_->rwlock = rwlock_new();
            stack_->semaphore = NULL;
            break;
        case 2:
            stack_->self.push = stack_push_blocking;
            stack_->self.pop = stack_pop_blocking;
            stack_->self.size = stack_size_blocking;
            stack_->rwlock = NULL;
            stack_->semaphore = semaphore_new(0);
            break;
    }

    // init size and head and comperator
    stack_->size = 0;
    stack_->head = memory_alloc(sizeof(struct StackItem));
    stack_->head->next = NULL;
    stack_->comperator = comperator;

    return stack_;
}
void stack_free(struct Stack* stack) {
    struct Stack_* stack_ = stack;

    // iterate stack and remove stackitems
    struct StackItem* remove_item = NULL;
    do {
        remove_item = stack_->head;
        stack_->head = stack_->head->next;
        memory_free(remove_item);
    } while (stack_->head != NULL);

    // destry internal rwlock
    if (stack_->rwlock != NULL) {
        rwlock_free(stack_->rwlock);
    }

    // destroy internal semaphore
    if (stack_->semaphore != NULL) {
        semaphore_free(stack_->semaphore);
    }

    memory_free(stack_);
}