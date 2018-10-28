#include <high/ThreadPool.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

struct Message{
    void (*function)(void*);
    int arg;
};

void test_function(void* arg) {
    struct Message* message = arg;

    assert(message->arg == 4);
}

void test_threadpool_start();
void test_threadpool_post();
void test_threadpool_stop();

void test_threadpool_start() {
    ThreadPool* threadpool = threadpool_new_object(2, sizeof(struct Message));

    assert(threadpool->vtable->start(threadpool) == 0);

    threadpool->vtable->stop(threadpool);

    threadpool_free(threadpool);
}
void test_threadpool_post() {
    ThreadPool* threadpool = threadpool_new_object(2, sizeof(struct Message));

    threadpool->vtable->start(threadpool);

    for (int a = 0; a < 20000; a++) {

        struct Message message = {
                test_function,
                4
        };

        assert(threadpool->vtable->post(threadpool, &message) == 0);
    }

    threadpool->vtable->stop(threadpool);

    threadpool_free(threadpool);
}
void test_threadpool_stop() {
    ThreadPool* threadpool = threadpool_new_object(2, sizeof(struct Message));

    threadpool->vtable->start(threadpool);

    assert(threadpool->vtable->stop(threadpool) == 0);

    threadpool_free(threadpool);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_threadpool_start();
        test_threadpool_post();
        test_threadpool_stop();
    }
}