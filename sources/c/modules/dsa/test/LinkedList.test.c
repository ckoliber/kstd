#include <high/LinkedList.h>
#include <low/Heap.h>

#include <assert.h>

int main() {
    LinkedList* linkedlist_1 = linkedlist_new_object(0, NULL);

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_1->vtable->add(linkedlist_1, item);
    }

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = linkedlist_1->vtable->get(linkedlist_1, cursor);
        assert(*item == cursor);
    }

    while (linkedlist_1->vtable->size(linkedlist_1) > 0) {
        heap_free(linkedlist_1->vtable->remove(linkedlist_1, 0));
    }

    linkedlist_free(linkedlist_1);
}