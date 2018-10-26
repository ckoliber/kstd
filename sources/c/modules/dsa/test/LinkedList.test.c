#include <high/LinkedList.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

int test_comperator(uint_8* item1, uint_8* item2) {
    return *(int*)item1 == *(int*)item2;
}

void test_linkedlist_add();
void test_linkedlist_addto();
void test_linkedlist_put();
void test_linkedlist_remove();
void test_linkedlist_get();
void test_linkedlist_indexof();
void test_linkedlist_size();

void test_linkedlist_add() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(linkedlist_normal->vtable->add(linkedlist_normal, (uint_8*)item) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_normal->vtable->get(linkedlist_normal, cursor);
        assert(*item == cursor);
    }

    while (linkedlist_normal->vtable->size(linkedlist_normal) > 0) {
        heap_free(linkedlist_normal->vtable->remove(linkedlist_normal, 0));
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(linkedlist_concurrent->vtable->add(linkedlist_concurrent, (uint_8*)item) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_concurrent->vtable->get(linkedlist_concurrent, cursor);
        assert(*item == cursor);
    }

    while (linkedlist_concurrent->vtable->size(linkedlist_concurrent) > 0) {
        heap_free(linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0));
    }

    linkedlist_free(linkedlist_concurrent);
}
void test_linkedlist_addto() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(linkedlist_normal->vtable->addto(linkedlist_normal, 0, (uint_8*)item) == 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_normal->vtable->get(linkedlist_normal, cursor);
        assert(*item == 999 - cursor);
    }

    while (linkedlist_normal->vtable->size(linkedlist_normal) > 0) {
        heap_free(linkedlist_normal->vtable->remove(linkedlist_normal, 0));
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        assert(linkedlist_concurrent->vtable->addto(linkedlist_concurrent, 0, (uint_8*)item) == 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_concurrent->vtable->get(linkedlist_concurrent, cursor);
        assert(*item == 999 - cursor);
    }

    while (linkedlist_concurrent->vtable->size(linkedlist_concurrent) > 0) {
        heap_free(linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0));
    }

    linkedlist_free(linkedlist_concurrent);
}
void test_linkedlist_put() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_normal->vtable->add(linkedlist_normal, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = 1418;
        heap_free(linkedlist_normal->vtable->put(linkedlist_normal, cursor, (uint_8*)item));
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_normal->vtable->get(linkedlist_normal, cursor);
        assert(*item == 1418);
    }

    while (linkedlist_normal->vtable->size(linkedlist_normal) > 0) {
        heap_free(linkedlist_normal->vtable->remove(linkedlist_normal, 0));
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_concurrent->vtable->add(linkedlist_concurrent, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = 1418;
        heap_free(linkedlist_concurrent->vtable->put(linkedlist_concurrent, cursor, (uint_8*)item));
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_concurrent->vtable->get(linkedlist_concurrent, cursor);
        assert(*item == 1418);
    }

    while (linkedlist_concurrent->vtable->size(linkedlist_concurrent) > 0) {
        heap_free(linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0));
    }

    linkedlist_free(linkedlist_concurrent);
}
void test_linkedlist_remove() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_normal->vtable->add(linkedlist_normal, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_normal->vtable->remove(linkedlist_normal, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_concurrent->vtable->add(linkedlist_concurrent, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0);
        assert(*item == cursor);
        heap_free((uint_8*)item);
    }

    linkedlist_free(linkedlist_concurrent);
}
void test_linkedlist_get() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_normal->vtable->add(linkedlist_normal, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_normal->vtable->get(linkedlist_normal, cursor);
        assert(*item == cursor);
    }

    while (linkedlist_normal->vtable->size(linkedlist_normal) > 0) {
        heap_free(linkedlist_normal->vtable->remove(linkedlist_normal, 0));
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_concurrent->vtable->add(linkedlist_concurrent, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)linkedlist_concurrent->vtable->get(linkedlist_concurrent, cursor);
        assert(*item == cursor);
    }

    while (linkedlist_concurrent->vtable->size(linkedlist_concurrent) > 0) {
        heap_free(linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0));
    }

    linkedlist_free(linkedlist_concurrent);
}
void test_linkedlist_indexof() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_normal->vtable->add(linkedlist_normal, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(linkedlist_normal->vtable->indexof(linkedlist_normal, (uint_8*)&cursor) == cursor);
    }

    while (linkedlist_normal->vtable->size(linkedlist_normal) > 0) {
        heap_free(linkedlist_normal->vtable->remove(linkedlist_normal, 0));
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_concurrent->vtable->add(linkedlist_concurrent, (uint_8*)item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(linkedlist_concurrent->vtable->indexof(linkedlist_concurrent, (uint_8*)&cursor) == cursor);
    }

    while (linkedlist_concurrent->vtable->size(linkedlist_concurrent) > 0) {
        heap_free(linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0));
    }

    linkedlist_free(linkedlist_concurrent);
}
void test_linkedlist_size() {
    // test normal linkedlist
    LinkedList* linkedlist_normal = linkedlist_new_object(0, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_normal->vtable->add(linkedlist_normal, (uint_8*)item);
    }

    assert(linkedlist_normal->vtable->size(linkedlist_normal) == 1000);

    while (linkedlist_normal->vtable->size(linkedlist_normal) > 0) {
        heap_free(linkedlist_normal->vtable->remove(linkedlist_normal, 0));
    }

    linkedlist_free(linkedlist_normal);

    // test concurrent linkedlist
    LinkedList* linkedlist_concurrent = linkedlist_new_object(1, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = (int*)heap_alloc(sizeof(int));
        *item = cursor;
        linkedlist_concurrent->vtable->add(linkedlist_concurrent, (uint_8*)item);
    }

    assert(linkedlist_concurrent->vtable->size(linkedlist_concurrent) == 1000);

    while (linkedlist_concurrent->vtable->size(linkedlist_concurrent) > 0) {
        heap_free(linkedlist_concurrent->vtable->remove(linkedlist_concurrent, 0));
    }

    linkedlist_free(linkedlist_concurrent);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        test_linkedlist_add();
        test_linkedlist_addto();
        test_linkedlist_put();
        test_linkedlist_remove();
        test_linkedlist_get();
        test_linkedlist_indexof();
        test_linkedlist_size();
    }
}