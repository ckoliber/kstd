#include <low/processor/low/Thread.h>
#include <low/itc/high/Lock.h>

#include <assert.h>
#include <zconf.h>

void* thread1(void* arg){
    Lock* l = arg;

    int result = l->lock(l);

    assert(result == 0);

    return NULL;
}

void* thread2(void* arg){
    Lock* l = arg;

    int result = l->timelock(l, 3000);

    assert(result == -1);

    return NULL;
}

int main(int argc, char* argv[]) {
    Lock *l1 = lock_new();
    Lock *l2 = lock_new();

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    l1->lock(l1);
    l2->lock(l2);

    t1->start(t1, thread1, l1);
    t2->start(t2, thread2, l2);

    sleep(5);

    l1->unlock(l1);
    l2->unlock(l2);

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    lock_free(l1);
    lock_free(l2);
}