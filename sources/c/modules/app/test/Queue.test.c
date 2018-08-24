#include <low/processor/low/Thread.h>
#include <low/dsa/Queue.h>

#include <stdio.h>
#include <io/memory/Memory.h>
#include <zconf.h>

void* thread1(void* arg){
    Queue* q = arg;

    while(1){
        int* item = q->dequeue(q, 5000);
        if(item == NULL){
            break;
        }
        printf("Thread 1 Remove Item = %d\n", *item);
        memory_free(item);
    }

    printf("Thread 1 End\n");

    return NULL;
}

void* thread2(void* arg){
    Queue* q = arg;

    while(q->size(q) > 0){
        int* item = q->dequeue(q, 5000);
        printf("Thread 2 Remove Item = %d\n", *item);
        memory_free(item);
    }

    printf("Thread 2 End\n");

    return NULL;
}

int main(int argc, char* argv[]) {
    Queue* q = queue_new(2, -1, NULL);

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    for(int cursor = 0 ; cursor < 100 ; cursor++){
        int* item = memory_alloc(sizeof(int));
        *item = cursor;
        q->enqueue(q, item);
    }

    t1->start(t1, thread1, q);
    t2->start(t2, thread2, q);

    sleep(3);

    for(int cursor = 0 ; cursor < 10000 ; cursor++){
        int* item = memory_alloc(sizeof(int));
        *item = cursor;
        q->enqueue(q, item);
    }

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    queue_free(q);
}