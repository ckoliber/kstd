#include <high/Stack.h>
#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

int main() {
    kstd_init();

    Stack* stack_1 = stack_new_object(0, -1, NULL);

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        stack_1->vtable->push(stack_1, item, 0);
    }

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = stack_1->vtable->pop(stack_1, 0);
        assert(*item == cursor);
        heap_free(item);
    }

    queue_free(stack_1);
}