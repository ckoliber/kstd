#include <high/Dequeue.h>

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

void test_dequeue_enqueue();
void test_dequeue_dequeue();
void test_dequeue_get();
void test_dequeue_size();

void test_dequeue_enqueue() {
    // test normal dequeue
    Dequeue* dequeue_normal = dequeue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(dequeue_normal->vtable->enqueue(dequeue_normal, 1, (uint_8*)item, 0) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)dequeue_normal->vtable->dequeue(dequeue_normal, 0, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    dequeue_free(dequeue_normal);

    // test concurrent dequeue
    Dequeue* dequeue_concurrent = dequeue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(dequeue_concurrent->vtable->enqueue(dequeue_concurrent, 1, (uint_8*)item, 0) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)dequeue_concurrent->vtable->dequeue(dequeue_concurrent, 0, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    dequeue_free(dequeue_concurrent);

    // test blocking dequeue
    Dequeue* dequeue_blocking = dequeue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(dequeue_blocking->vtable->enqueue(dequeue_blocking, 1, (uint_8*)item, 0) == 999 - cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)dequeue_blocking->vtable->dequeue(dequeue_blocking, 0, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    dequeue_free(dequeue_blocking);
}
void test_dequeue_dequeue() {
    // test normal dequeue
    Dequeue* dequeue_normal = dequeue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_normal->vtable->enqueue(dequeue_normal, 1, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)dequeue_normal->vtable->dequeue(dequeue_normal, 0, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    dequeue_free(dequeue_normal);

    // test concurrent dequeue
    Dequeue* dequeue_concurrent = dequeue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_concurrent->vtable->enqueue(dequeue_concurrent, 1, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)dequeue_concurrent->vtable->dequeue(dequeue_concurrent, 0, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    dequeue_free(dequeue_concurrent);

    // test blocking dequeue
    Dequeue* dequeue_blocking = dequeue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_blocking->vtable->enqueue(dequeue_blocking, 1, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)dequeue_blocking->vtable->dequeue(dequeue_blocking, 0, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    dequeue_free(dequeue_blocking);
}
void test_dequeue_get() {
    // test normal dequeue
    Dequeue* dequeue_normal = dequeue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_normal->vtable->enqueue(dequeue_normal, 1, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)dequeue_normal->vtable->get(dequeue_normal, 0) == cursor);

        heap_free(dequeue_normal->vtable->dequeue(dequeue_normal, 0, 0));
    }

    dequeue_free(dequeue_normal);

    // test concurrent dequeue
    Dequeue* dequeue_concurrent = dequeue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_concurrent->vtable->enqueue(dequeue_concurrent, 1, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)dequeue_concurrent->vtable->get(dequeue_concurrent, 0) == cursor);

        heap_free(dequeue_concurrent->vtable->dequeue(dequeue_concurrent, 0, 0));
    }

    dequeue_free(dequeue_concurrent);

    // test blocking dequeue
    Dequeue* dequeue_blocking = dequeue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_blocking->vtable->enqueue(dequeue_blocking, 1, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)dequeue_blocking->vtable->get(dequeue_blocking, 0) == cursor);

        heap_free(dequeue_blocking->vtable->dequeue(dequeue_blocking, 0, 0));
    }

    dequeue_free(dequeue_blocking);
}
void test_dequeue_size() {
    // test normal dequeue
    Dequeue* dequeue_normal = dequeue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_normal->vtable->enqueue(dequeue_normal, 1, (uint_8*)item, 0);
    }

    assert(dequeue_normal->vtable->size(dequeue_normal) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(dequeue_normal->vtable->dequeue(dequeue_normal, 0, 0));
    }

    dequeue_free(dequeue_normal);

    // test concurrent dequeue
    Dequeue* dequeue_concurrent = dequeue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_concurrent->vtable->enqueue(dequeue_concurrent, 1, (uint_8*)item, 0);
    }

    assert(dequeue_concurrent->vtable->size(dequeue_concurrent) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(dequeue_concurrent->vtable->dequeue(dequeue_concurrent, 0, 0));
    }

    dequeue_free(dequeue_concurrent);

    // test blocking dequeue
    Dequeue* dequeue_blocking = dequeue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        dequeue_blocking->vtable->enqueue(dequeue_blocking, 1, (uint_8*)item, 0);
    }

    assert(dequeue_blocking->vtable->size(dequeue_blocking) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(dequeue_blocking->vtable->dequeue(dequeue_blocking, 0, 0));
    }

    dequeue_free(dequeue_blocking);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        test_dequeue_enqueue();
        test_dequeue_dequeue();
        test_dequeue_get();
        test_dequeue_size();
    }
}