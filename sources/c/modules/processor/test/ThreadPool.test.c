#include <high/ThreadPool.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>
#include <stdio.h>

void test_function(void* arg) {
    assert((int) arg == 4);
}

void test_threadpool_start();
void test_threadpool_post();
void test_threadpool_stop();

void test_threadpool_start(){
    ThreadPool* threadpool = threadpool_new_object(2, sizeof(int));

    assert(threadpool->vtable->start(threadpool) == 0);

    threadpool->vtable->stop(threadpool);

    threadpool_free(threadpool);
}
void test_threadpool_post(){
    ThreadPool* threadpool = threadpool_new_object(2, sizeof(int));

    threadpool->vtable->start(threadpool);

    for (int a = 0; a < 2000; a++) {
        int arg = 4;
        threadpool->vtable->post(threadpool, test_function, &arg);
    }

    threadpool->vtable->stop(threadpool);

    threadpool_free(threadpool);
}
void test_threadpool_stop(){
    ThreadPool* threadpool = threadpool_new_object(2, sizeof(int));

    threadpool->vtable->start(threadpool);

    assert(threadpool->vtable->stop(threadpool) == 0);

    threadpool_free(threadpool);
}

int main() {
    kstd_init();

    test_threadpool_start();
    test_threadpool_post();
    test_threadpool_stop();
}