#include <low/processor/low/Thread.h>
#include <low/itc/high/RWLock.h>

#include <assert.h>
#include <zconf.h>

void* thread1(void* arg){
    RWLock* rw = arg;

    int result = rw->readlock(rw);

    assert(result == 0);

    rw->readunlock(rw);

    return NULL;
}

void* thread2(void* arg){
    RWLock* rw = arg;

    int result = rw->timereadlock(rw, 3000);

    assert(result == 0);

    rw->readunlock(rw);

    return NULL;
}

void* thread3(void* arg){
    RWLock* rw = arg;

    int result = rw->writelock(rw);

    assert(result == 0);

    return NULL;
}

void* thread4(void* arg){
    RWLock* rw = arg;

    int result = rw->timewritelock(rw, 3000);

    assert(result == -1);

    return NULL;
}

int main(int argc, char* argv[]) {
    RWLock *rw = rwlock_new();

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();
    Thread* t3 = thread_new();
    Thread* t4 = thread_new();

    rw->readlock(rw);

    t1->start(t1, thread1, rw);
    t2->start(t2, thread2, rw);
    t2->start(t3, thread3, rw);
    t2->start(t4, thread4, rw);

    sleep(5);

    rw->readunlock(rw);

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

    rwlock_free(rw);
}