#include <high/Queue.h>
#include <low/Heap.h>

#include <assert.h>

int main() {
    Queue* queue_1 = queue_new_object(0, -1, NULL);

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        queue_1->vtable->enqueue(queue_1, item, 0);
    }

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = queue_1->vtable->dequeue(queue_1, 0);
        assert(*item == cursor);
        heap_free(item);
    }

    queue_free(queue_1);
}