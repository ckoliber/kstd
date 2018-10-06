#include <low/Heap.h>

#include <assert.h>

int main() {
    char* heap_1 = heap_alloc(1024);
    char* heap_2 = heap_alloc(1024);

    assert(heap_1 != NULL);
    assert(heap_2 != NULL);

    heap_set(heap_1, 1, 1024);
    heap_set(heap_2, 2, 1024);

    for (int cursor = 0; cursor < 1024; cursor++) {
        assert(heap_1[cursor] == 1);
    }
    for (int cursor = 0; cursor < 1024; cursor++) {
        assert(heap_2[cursor] == 2);
    }

    heap_copy(heap_1, heap_2, 1024);

    for (int cursor = 0; cursor < 1024; cursor++) {
        assert(heap_1[cursor] == 2);
    }

    heap_1 = heap_realloc(heap_1, 2048);
    heap_2 = heap_realloc(heap_2, 2048);

    assert(heap_1 != NULL);
    assert(heap_2 != NULL);

    heap_free(heap_1);
    heap_free(heap_2);
}