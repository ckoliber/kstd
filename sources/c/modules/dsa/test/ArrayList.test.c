#include <high/ArrayList.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>
#include <stdio.h>

int test_comperator(void* item1, void* item2){
    return *(int*) item1 == *(int*) item2;
}

void test_arraylist_add();
void test_arraylist_addto();
void test_arraylist_put();
void test_arraylist_remove();
void test_arraylist_get();
void test_arraylist_indexof();
void test_arraylist_size();

void test_arraylist_add(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        assert(arraylist_normal->vtable->add(arraylist_normal, item) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_normal->vtable->get(arraylist_normal, cursor);
        assert(*item == cursor);
    }

    while (arraylist_normal->vtable->size(arraylist_normal) > 0) {
        heap_free(arraylist_normal->vtable->remove(arraylist_normal, 0));
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        assert(arraylist_concurrent->vtable->add(arraylist_concurrent, item) == cursor);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_concurrent->vtable->get(arraylist_concurrent, cursor);
        assert(*item == cursor);
    }

    while (arraylist_concurrent->vtable->size(arraylist_concurrent) > 0) {
        heap_free(arraylist_concurrent->vtable->remove(arraylist_concurrent, 0));
    }

    arraylist_free(arraylist_concurrent);
}
void test_arraylist_addto(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        assert(arraylist_normal->vtable->addto(arraylist_normal, 0, item) == 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_normal->vtable->get(arraylist_normal, cursor);
        assert(*item == 999 - cursor);
    }

    while (arraylist_normal->vtable->size(arraylist_normal) > 0) {
        heap_free(arraylist_normal->vtable->remove(arraylist_normal, 0));
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        assert(arraylist_concurrent->vtable->addto(arraylist_concurrent, 0, item) == 0);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_concurrent->vtable->get(arraylist_concurrent, cursor);
        assert(*item == 999 - cursor);
    }

    while (arraylist_concurrent->vtable->size(arraylist_concurrent) > 0) {
        heap_free(arraylist_concurrent->vtable->remove(arraylist_concurrent, 0));
    }

    arraylist_free(arraylist_concurrent);
}
void test_arraylist_put(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_normal->vtable->add(arraylist_normal, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = 1418;
        heap_free(arraylist_normal->vtable->put(arraylist_normal, cursor, item));
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_normal->vtable->get(arraylist_normal, cursor);
        assert(*item == 1418);
    }

    while (arraylist_normal->vtable->size(arraylist_normal) > 0) {
        heap_free(arraylist_normal->vtable->remove(arraylist_normal, 0));
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_concurrent->vtable->add(arraylist_concurrent, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = 1418;
        heap_free(arraylist_concurrent->vtable->put(arraylist_concurrent, cursor, item));
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_concurrent->vtable->get(arraylist_concurrent, cursor);
        assert(*item == 1418);
    }

    while (arraylist_concurrent->vtable->size(arraylist_concurrent) > 0) {
        heap_free(arraylist_concurrent->vtable->remove(arraylist_concurrent, 0));
    }

    arraylist_free(arraylist_concurrent);
}
void test_arraylist_remove(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_normal->vtable->add(arraylist_normal, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_normal->vtable->remove(arraylist_normal, 0);
        assert(*item == cursor);
        heap_free(item);
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_concurrent->vtable->add(arraylist_concurrent, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_concurrent->vtable->remove(arraylist_concurrent, 0);
        assert(*item == cursor);
        heap_free(item);
    }

    arraylist_free(arraylist_concurrent);
}
void test_arraylist_get(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_normal->vtable->add(arraylist_normal, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_normal->vtable->get(arraylist_normal, cursor);
        assert(*item == cursor);
    }

    while (arraylist_normal->vtable->size(arraylist_normal) > 0) {
        heap_free(arraylist_normal->vtable->remove(arraylist_normal, 0));
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, NULL);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_concurrent->vtable->add(arraylist_concurrent, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = arraylist_concurrent->vtable->get(arraylist_concurrent, cursor);
        assert(*item == cursor);
    }

    while (arraylist_concurrent->vtable->size(arraylist_concurrent) > 0) {
        heap_free(arraylist_concurrent->vtable->remove(arraylist_concurrent, 0));
    }

    arraylist_free(arraylist_concurrent);
}
void test_arraylist_indexof(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_normal->vtable->add(arraylist_normal, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(arraylist_normal->vtable->indexof(arraylist_normal, &cursor) == cursor);
    }

    while (arraylist_normal->vtable->size(arraylist_normal) > 0) {
        heap_free(arraylist_normal->vtable->remove(arraylist_normal, 0));
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_concurrent->vtable->add(arraylist_concurrent, item);
    }

    for (int cursor = 0; cursor < 1000; cursor++) {
        assert(arraylist_concurrent->vtable->indexof(arraylist_concurrent, &cursor) == cursor);
    }

    while (arraylist_concurrent->vtable->size(arraylist_concurrent) > 0) {
        heap_free(arraylist_concurrent->vtable->remove(arraylist_concurrent, 0));
    }

    arraylist_free(arraylist_concurrent);
}
void test_arraylist_size(){
    // test normal arraylist
    ArrayList* arraylist_normal = arraylist_new_object(0, 1, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_normal->vtable->add(arraylist_normal, item);
    }

    assert(arraylist_normal->vtable->size(arraylist_normal) == 1000);

    while (arraylist_normal->vtable->size(arraylist_normal) > 0) {
        heap_free(arraylist_normal->vtable->remove(arraylist_normal, 0));
    }

    arraylist_free(arraylist_normal);

    // test concurrent arraylist
    ArrayList* arraylist_concurrent = arraylist_new_object(1, 2, test_comperator);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        arraylist_concurrent->vtable->add(arraylist_concurrent, item);
    }

    assert(arraylist_concurrent->vtable->size(arraylist_concurrent) == 1000);

    while (arraylist_concurrent->vtable->size(arraylist_concurrent) > 0) {
        heap_free(arraylist_concurrent->vtable->remove(arraylist_concurrent, 0));
    }

    arraylist_free(arraylist_concurrent);
}

int main() {
    kstd_init();

    test_arraylist_add();
    test_arraylist_addto();
    test_arraylist_put();
    test_arraylist_remove();
    test_arraylist_get();
    test_arraylist_indexof();
    test_arraylist_size();
}