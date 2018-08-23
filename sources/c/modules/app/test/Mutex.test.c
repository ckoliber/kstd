#include <low/processor/low/Thread.h>
#include <low/itc/low/Mutex.h>

#include <assert.h>
#include <zconf.h>

void* thread1(void* arg){
    Mutex* m = arg;

    int result = m->lock(m);

    assert(result == 0);

    return NULL;
}

void* thread2(void* arg){
    Mutex* m = arg;

    int result = m->timelock(m, 3000);

    assert(result == -1);

    return NULL;
}

int main(int argc, char* argv[]) {
    Mutex *m1 = mutex_new();
    Mutex *m2 = mutex_new();

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    m1->lock(m1);
    m2->lock(m2);

    t1->start(t1, thread1, m1);
    t2->start(t2, thread2, m2);

    sleep(5);

    m1->unlock(m1);
    m2->unlock(m2);

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    mutex_free(m1);
    mutex_free(m2);
}