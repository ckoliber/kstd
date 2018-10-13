#include <high/ArrayList.h>
#include <low/Heap.h>

#include <assert.h>

int main() {
    ArrayList* arraylist_1 = arraylist_new_object(0, 1, NULL);

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_1->vtable->add(arraylist_1, item);
    }

    for (int cursor = 0; cursor < 10000; cursor++) {
        int* item = arraylist_1->vtable->get(arraylist_1, cursor);
        assert(*item == cursor);
    }

    while (arraylist_1->vtable->size(arraylist_1) > 0) {
        heap_free(arraylist_1->vtable->remove(arraylist_1, 0));
    }

    arraylist_free(arraylist_1);
}