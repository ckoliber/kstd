#include <low/dsa/ArrayList.h>
#include <low/processor/low/Thread.h>

#include <assert.h>
#include <stdio.h>

void* thread(void* arg) {
    printf("\n%d\n", arg);

    return NULL;
}

int main(int argc, char* argv[]) {
    printf("\nA\n");
    fflush(stdout);

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    printf("\nB\n");
    fflush(stdout);

    t1->start(t1, thread, 1);
    t2->start(t2, thread, 2);

    printf("\nC\n");
    fflush(stdout);

    t1->join(t1);
    // t2->join(t2);

    printf("\nD\n");
    fflush(stdout);

    t1->stop(t1);
    // t2->stop(t2);

    printf("\nE\n");
    fflush(stdout);

    thread_free(t1);
    thread_free(t2);

    printf("\nF\n");
    fflush(stdout);

    // printf("X\n");
    // fflush(stdout);
    // ArrayList* list = arraylist_new(0, 2, NULL);
    // printf("X\n");
    // fflush(stdout);
    // list->add(list, 1);
    // list->add(list, 2);
    // list->add(list, 3);
    // printf("X\n");
    // fflush(stdout);

    // printf("%d\n", list->remove(list, 0));
    // printf("%d\n", list->remove(list, 0));
    // printf("%d\n", list->remove(list, 0));
}