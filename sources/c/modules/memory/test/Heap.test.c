#include <low/Heap.h>

#include <assert.h>

void test_heap_alloc();
void test_heap_realloc();
void test_heap_free();
void test_heap_copy();
void test_heap_move();
void test_heap_set();

void test_heap_alloc(){
    uint_8* heap = heap_alloc(1024);

    assert(heap != NULL);

    heap_free(heap);
}
void test_heap_realloc(){
    uint_8* heap = heap_alloc(1024);

    heap = heap_realloc(heap, 2048);

    assert(heap != NULL);

    heap_free(heap);
}
void test_heap_free(){
    void* heap = heap_alloc(1024);

    heap_free(heap);

    assert(heap != NULL);
}
void test_heap_copy(){
    int* heap = (int*) heap_alloc(sizeof(int) * 1024);

    int* heap2 = (int*) heap_alloc(sizeof(int) * 1024);

    heap_copy((uint_8 *) heap, (uint_8 *) heap2, sizeof(int) * 1024);

    for(int cursor = 0 ; cursor < 1024 ; cursor++){
        assert(heap[cursor] == heap2[cursor]);
    }

    heap_free((uint_8 *) heap);

    heap_free((uint_8 *) heap2);
}
void test_heap_move(){
    int* heap = (int *) heap_alloc(sizeof(int) * 1024);

    int* heap2 = (int *) heap_alloc(sizeof(int) * 1024);

    heap_move((uint_8 *) heap, (uint_8 *) heap2, sizeof(int) * 1024);

    for(int cursor = 0 ; cursor < 1024 ; cursor++){
        assert(heap[cursor] == heap2[cursor]);
    }

    heap_free((uint_8 *) heap);

    heap_free((uint_8 *) heap2);
}
void test_heap_set(){
    char* heap = (char *) heap_alloc(sizeof(char) * 1024);

    heap_set((uint_8 *) heap, 5, 1024);

    for(int cursor = 0 ; cursor < 1024 ; cursor++){
        assert(heap[cursor] == 5);
    }

    heap_free((uint_8 *) heap);
}

int main() {
    test_heap_alloc();
    test_heap_realloc();
    test_heap_free();
    test_heap_copy();
    test_heap_move();
    test_heap_set();
}
