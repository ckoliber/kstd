#include <low/itc/high/Semaphore.h>

#include <assert.h>
#include <low/processor/low/Thread.h>
#include <zconf.h>

void* thread1(void* arg) {
    Semaphore* s = arg;

    int result = s->wait(s, 3);

    assert(result == 0);

    return NULL;
}

void* thread2(void* arg) {
    Semaphore* s = arg;

    int result = s->timewait(s, 3, 3000);

    assert(result == -1);

    return NULL;
}

int main(int argc, char* argv[]) {
    Semaphore* s = semaphore_new(2);

    Thread* t1 = thread_new();
    Thread* t2 = thread_new();

    t1->start(t1, thread1, s);
    t2->start(t2, thread2, s);

    sleep(5);

    s->post(s, 1);

    t1->join(t1);
    t2->join(t2);

    t1->stop(t1);
    t2->stop(t2);

    thread_free(t1);
    thread_free(t2);

    semaphore_free(s);
}