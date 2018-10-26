#include <high/Stack.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

int test_comperator(uint_8* item1, uint_8* item2) {
    if (*(int*)item1 < *(int*)item2) {
        return 1;
    } else if (*(int*)item1 > *(int*)item2) {
        return -1;
    } else {
        return 0;
    }
}

void test_stack_push();
void test_stack_pop();
void test_stack_get();
void test_stack_size();

void test_stack_push() {
    // test normal stack
    Stack* stack_normal = stack_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(stack_normal->vtable->push(stack_normal, (uint_8*)item, 0) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)stack_normal->vtable->pop(stack_normal, 0);
        assert(*item == 999 - cursor);
        heap_free((uint_8*)item);
    }

    stack_free(stack_normal);

    // test concurrent stack
    Stack* stack_concurrent = stack_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(stack_concurrent->vtable->push(stack_concurrent, (uint_8*)item, 0) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)stack_concurrent->vtable->pop(stack_concurrent, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    stack_free(stack_concurrent);

    // test blocking stack
    Stack* stack_blocking = stack_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(stack_blocking->vtable->push(stack_blocking, (uint_8*)item, 0) == 999 - cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)stack_blocking->vtable->pop(stack_blocking, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    stack_free(stack_blocking);
}
void test_stack_pop() {
    // test normal stack
    Stack* stack_normal = stack_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_normal->vtable->push(stack_normal, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)stack_normal->vtable->pop(stack_normal, 0);
        assert(*item == 999 - cursor);
        heap_free((uint_8*)item);
    }

    stack_free(stack_normal);

    // test concurrent stack
    Stack* stack_concurrent = stack_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_concurrent->vtable->push(stack_concurrent, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)stack_concurrent->vtable->pop(stack_concurrent, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    stack_free(stack_concurrent);

    // test blocking stack
    Stack* stack_blocking = stack_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_blocking->vtable->push(stack_blocking, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)stack_blocking->vtable->pop(stack_blocking, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    stack_free(stack_blocking);
}
void test_stack_get() {
    // test normal stack
    Stack* stack_normal = stack_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_normal->vtable->push(stack_normal, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)stack_normal->vtable->get(stack_normal) == 999 - cursor);

        heap_free(stack_normal->vtable->pop(stack_normal, 0));
    }

    stack_free(stack_normal);

    // test concurrent stack
    Stack* stack_concurrent = stack_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_concurrent->vtable->push(stack_concurrent, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)stack_concurrent->vtable->get(stack_concurrent) == cursor);

        heap_free(stack_concurrent->vtable->pop(stack_concurrent, 0));
    }

    stack_free(stack_concurrent);

    // test blocking stack
    Stack* stack_blocking = stack_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_blocking->vtable->push(stack_blocking, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)stack_blocking->vtable->get(stack_blocking) == cursor);

        heap_free(stack_blocking->vtable->pop(stack_blocking, 0));
    }

    stack_free(stack_blocking);
}
void test_stack_size() {
    // test normal stack
    Stack* stack_normal = stack_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_normal->vtable->push(stack_normal, (uint_8*)item, 0);
    }

    assert(stack_normal->vtable->size(stack_normal) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(stack_normal->vtable->pop(stack_normal, 0));
    }

    stack_free(stack_normal);

    // test concurrent stack
    Stack* stack_concurrent = stack_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_concurrent->vtable->push(stack_concurrent, (uint_8*)item, 0);
    }

    assert(stack_concurrent->vtable->size(stack_concurrent) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(stack_concurrent->vtable->pop(stack_concurrent, 0));
    }

    stack_free(stack_concurrent);

    // test blocking stack
    Stack* stack_blocking = stack_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        stack_blocking->vtable->push(stack_blocking, (uint_8*)item, 0);
    }

    assert(stack_blocking->vtable->size(stack_blocking) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(stack_blocking->vtable->pop(stack_blocking, 0));
    }

    stack_free(stack_blocking);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        test_stack_push();
        test_stack_pop();
        test_stack_get();
        test_stack_size();
    }
}