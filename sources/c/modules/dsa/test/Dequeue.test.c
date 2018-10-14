#include <high/Dequeue.h>
#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>
#include <stdio.h>

int main() {
    kstd_init();

    Dequeue* dequeue_1 = dequeue_new_object(0, -1, NULL);

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_1->vtable->enqueue(dequeue_1, 0, item, 0);
    }

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = dequeue_1->vtable->dequeue(dequeue_1, 1, 0);
        assert(*item == cursor);
        heap_free(item);
    }

    dequeue_free(dequeue_1);
}