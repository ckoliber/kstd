#include <high/Queue.h>

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

void test_queue_enqueue();
void test_queue_queue();
void test_queue_get();
void test_queue_size();

void test_queue_enqueue() {
    // test normal queue
    Queue* queue_normal = queue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(queue_normal->vtable->enqueue(queue_normal, (uint_8*)item, 0) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)queue_normal->vtable->dequeue(queue_normal, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    queue_free(queue_normal);

    // test concurrent queue
    Queue* queue_concurrent = queue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(queue_concurrent->vtable->enqueue(queue_concurrent, (uint_8*)item, 0) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)queue_concurrent->vtable->dequeue(queue_concurrent, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    queue_free(queue_concurrent);

    // test blocking queue
    Queue* queue_blocking = queue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(queue_blocking->vtable->enqueue(queue_blocking, (uint_8*)item, 0) == 999 - cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)queue_blocking->vtable->dequeue(queue_blocking, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    queue_free(queue_blocking);
}
void test_queue_queue() {
    // test normal queue
    Queue* queue_normal = queue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_normal->vtable->enqueue(queue_normal, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)queue_normal->vtable->dequeue(queue_normal, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    queue_free(queue_normal);

    // test concurrent queue
    Queue* queue_concurrent = queue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_concurrent->vtable->enqueue(queue_concurrent, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)queue_concurrent->vtable->dequeue(queue_concurrent, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    queue_free(queue_concurrent);

    // test blocking queue
    Queue* queue_blocking = queue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_blocking->vtable->enqueue(queue_blocking, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)queue_blocking->vtable->dequeue(queue_blocking, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    queue_free(queue_blocking);
}
void test_queue_get() {
    // test normal queue
    Queue* queue_normal = queue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_normal->vtable->enqueue(queue_normal, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)queue_normal->vtable->get(queue_normal) == cursor);

        heap_free(queue_normal->vtable->dequeue(queue_normal, 0));
    }

    queue_free(queue_normal);

    // test concurrent queue
    Queue* queue_concurrent = queue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_concurrent->vtable->enqueue(queue_concurrent, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)queue_concurrent->vtable->get(queue_concurrent) == cursor);

        heap_free(queue_concurrent->vtable->dequeue(queue_concurrent, 0));
    }

    queue_free(queue_concurrent);

    // test blocking queue
    Queue* queue_blocking = queue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_blocking->vtable->enqueue(queue_blocking, (uint_8*)item, 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(*(int*)queue_blocking->vtable->get(queue_blocking) == cursor);

        heap_free(queue_blocking->vtable->dequeue(queue_blocking, 0));
    }

    queue_free(queue_blocking);
}
void test_queue_size() {
    // test normal queue
    Queue* queue_normal = queue_new_object(0, 0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_normal->vtable->enqueue(queue_normal, (uint_8*)item, 0);
    }

    assert(queue_normal->vtable->size(queue_normal) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(queue_normal->vtable->dequeue(queue_normal, 0));
    }

    queue_free(queue_normal);

    // test concurrent queue
    Queue* queue_concurrent = queue_new_object(1, 0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_concurrent->vtable->enqueue(queue_concurrent, (uint_8*)item, 0);
    }

    assert(queue_concurrent->vtable->size(queue_concurrent) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(queue_concurrent->vtable->dequeue(queue_concurrent, 0));
    }

    queue_free(queue_concurrent);

    // test blocking queue
    Queue* queue_blocking = queue_new_object(2, 0, test_comperator);

    for (int cursor = 999; cursor >= 0; cursor--) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        queue_blocking->vtable->enqueue(queue_blocking, (uint_8*)item, 0);
    }

    assert(queue_blocking->vtable->size(queue_blocking) == 1000);

    for (int cursor = 0; cursor < 1000; cursor++) {
        heap_free(queue_blocking->vtable->dequeue(queue_blocking, 0));
    }

    queue_free(queue_blocking);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        test_queue_enqueue();
        test_queue_queue();
        test_queue_get();
        test_queue_size();
    }
}