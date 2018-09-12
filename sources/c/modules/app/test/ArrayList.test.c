#include <low/dsa/ArrayList.h>

#include <io/memory/Memory.h>
#include <low/processor/low/Thread.h>
#include <stdio.h>

void* thread1(void* arg) {
    ArrayList* a = arg;

    while (a->size(a) > 0) {
        int* item = a->remove(a, 0);
        printf("Thread 1 Remove Item = %d\n", *item);
        heap_free(item);
    }

    //    assert(result == 0);

    return NULL;
}

void* thread2(void* arg) {
    ArrayList* a = arg;

    while (a->size(a) > 0) {
        int* item = a->remove(a, 0);
        printf("Thread 2 Remove Item = %d\n", *item);
        heap_free(item);
    }

    //    assert(result == 0);

    return NULL;
}

void* thread3(void* arg) {
    ArrayList* a = arg;

    while (a->size(a) > 0) {
        int* item = a->remove(a, 0);
        printf("Thread 3 Remove Item = %d\n", *item);
        heap_free(item);
    }

    //    assert(result == 0);

    return NULL;
}

void* thread4(void* arg) {
    ArrayList* a = arg;

    while (a->size(a) > 0) {
        int* item = a->remove(a, 0);
        printf("Thread 4 Remove Item = %d\n", *item);
        heap_free(item);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    ArrayList* a = arraylist_new(1, 1.5, NULL);

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();
    Thread* t3 = thread_new();
    Thread* t4 = thread_new();

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        a->add(a, item);
    }

    t1->start(t1, thread1, a);
    t2->start(t2, thread2, a);
    t3->start(t3, thread3, a);
    t4->start(t4, thread4, a);

    t1->join(t1);
    t2->join(t2);
    t3->join(t3);
    t4->join(t4);

    t1->stop(t1);
    t2->stop(t2);
    t3->stop(t3);
    t4->stop(t4);

    thread_free(t1);
    thread_free(t2);
    thread_free(t3);
    thread_free(t4);

    arraylist_free(a);
}