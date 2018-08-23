#include <low/processor/low/Thread.h>

#include <assert.h>
#include <stdlib.h>

void* thread1(void* arg) {
    assert(arg == 1);
    return NULL;
}

void* thread2(void* arg) {
    assert(arg == 2);
    return NULL;
}

int main(int argc, char* argv[]) {
    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    t1->start(t1, thread1, (void *) 1);
    t2->start(t2, thread2, (void *) 2);

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    return 0;
}