#include <low/dsa/Stack.h>

#include <io/memory/Memory.h>
#include <low/processor/low/Thread.h>
#include <stdio.h>
#include <zconf.h>

void* thread1(void* arg) {
    Stack* s = arg;

    while (1) {
        int* item = s->pop(s, 5000);
        if (item == NULL) {
            break;
        }
        printf("Thread 1 Remove Item = %d\n", *item);
        memory_free(item);
    }

    printf("Thread 1 End\n");

    return NULL;
}

void* thread2(void* arg) {
    Stack* s = arg;

    while (s->size(s) > 0) {
        int* item = s->pop(s, 0);
        printf("Thread 2 Remove Item = %d\n", *item);
        memory_free(item);
    }

    printf("Thread 2 End\n");

    return NULL;
}

int main(int argc, char* argv[]) {
    Stack* s = stack_new(2, -1, NULL);

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = memory_alloc(sizeof(int));
        *item = cursor;
        s->push(s, item);
    }

    t1->start(t1, thread1, s);
    t2->start(t2, thread2, s);

    sleep(3);

    for (int cursor = 0; cursor < 1000; cursor++) {
        int* item = memory_alloc(sizeof(int));
        *item = cursor;
        s->push(s, item);
    }

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    stack_free(s);
}