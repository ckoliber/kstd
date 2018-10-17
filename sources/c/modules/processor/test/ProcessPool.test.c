#include <high/ProcessPool.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>
#include <stdio.h>

void test_function(void* arg) {
    assert((int) arg == 4);
}

void test_processpool_start();
void test_processpool_post();
void test_processpool_stop();

void test_processpool_start(){
    ProcessPool* processpool = processpool_new_object(2, sizeof(int));

    assert(processpool->vtable->start(processpool) == 0);

    processpool->vtable->stop(processpool);

    processpool_free(processpool);
}
void test_processpool_post(){
    ProcessPool* processpool = processpool_new_object(2, sizeof(int));

    processpool->vtable->start(processpool);

    for (int a = 0; a < 100000; a++) {
        int arg = 4;
        processpool->vtable->post(processpool, test_function, &arg);
    }

    processpool->vtable->stop(processpool);

    processpool_free(processpool);
}
void test_processpool_stop(){
    ProcessPool* processpool = processpool_new_object(2, sizeof(int));

    processpool->vtable->start(processpool);

    assert(processpool->vtable->stop(processpool) == 0);

    processpool_free(processpool);
}

int main() {
    kstd_init();

    test_processpool_start();
    test_processpool_post();
    test_processpool_stop();
}