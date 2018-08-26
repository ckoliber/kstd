#include <low/itc/low/Cond.h>

#include <assert.h>
#include <low/processor/low/Thread.h>
#include <zconf.h>

void* thread1(void* arg) {
    Cond* c = arg;

    int result = c->wait(c, NULL, NULL, NULL, NULL);

    assert(result == 0);

    return NULL;
}

void* thread2(void* arg) {
    Cond* c = arg;

    int result = c->timewait(c, NULL, NULL, NULL, NULL, 3000);

    assert(result == -1);

    return NULL;
}

int main(int argc, char* argv[]) {
    Cond* c1 = cond_new();
    Cond* c2 = cond_new();

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    t1->start(t1, thread1, c1);
    t2->start(t2, thread2, c2);

    sleep(5);

    c1->signal(c1, NULL, NULL);
    c2->signal(c2, NULL, NULL);

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    cond_free(c1);
    cond_free(c2);
}