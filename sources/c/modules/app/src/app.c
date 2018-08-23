#include <low/processor/low/Thread.h>
#include <low/dsa/Dequeue.h>

#include <stdio.h>
#include <io/memory/Memory.h>

void* thread1(void* arg){
    Queue* q = arg;

    while(q->size(q) > 0){
//        int* item = q->remove(l, 0);
//        printf("Thread 1 Remove Item = %d\n", *item);
//        memory_free(item);
    }

//    assert(result == 0);

    return NULL;
}


int main(int argc, char* argv[]) {
    Queue* q = queue_new(0, NULL);

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();
    Thread* t3 = thread_new();
    Thread* t4 = thread_new();

//    t1->start(t1, thread1, l);
//    t2->start(t2, thread2, l);
//    t3->start(t3, thread3, l);
//    t4->start(t4, thread4, l);

//    t1->join(t1);
//    t2->join(t2);
//    t3->join(t3);
//    t4->join(t4);

//    t1->stop(t1);
//    t2->stop(t2);
//    t3->stop(t3);
//    t4->stop(t4);

    thread_free(t1);
    thread_free(t2);
    thread_free(t3);
    thread_free(t4);

    queue_free(q);
}