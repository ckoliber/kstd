#include <high/ProcessPool.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>
#include <stdio.h>

struct Message{
    void (*function)(uint_8*);
    int arg;
};

void test_function(uint_8* arg) {
    struct Message* message = (struct Message*) arg;

    printf("Recv\n");
    fflush(stdout);

    assert(message->arg == 4);
}

void test_processpool_start();
void test_processpool_post();
void test_processpool_stop();

void test_processpool_start() {
    printf("1\n");
    fflush(stdout);

    ProcessPool* processpool = processpool_new_object(2, sizeof(struct Message));

    printf("2\n");
    fflush(stdout);

    assert(processpool->vtable->start(processpool) == 0);

    printf("3\n");
    fflush(stdout);

    processpool->vtable->stop(processpool);

    printf("4\n");
    fflush(stdout);

    processpool_free(processpool);

    printf("5\n");
    fflush(stdout);
}
void test_processpool_post() {
    ProcessPool* processpool = processpool_new_object(1, sizeof(struct Message));

    processpool->vtable->start(processpool);

    for (int a = 0; a < 2000; a++) {
        printf("Send\n");
        struct Message message = {
                test_function,
                4
        };
        processpool->vtable->post(processpool, (uint_8*) &message);
    }



    processpool->vtable->stop(processpool);

    processpool_free(processpool);
}
void test_processpool_stop() {
    ProcessPool* processpool = processpool_new_object(2, sizeof(struct Message));

    processpool->vtable->start(processpool);

    assert(processpool->vtable->stop(processpool) == 0);

    processpool_free(processpool);
}

int main() {
    kstd_init();

    printf("Test Start\n");
    fflush(stdout);

    test_processpool_start();

    printf("Test Post\n");
    fflush(stdout);

    test_processpool_post();

    printf("Test Stop\n");
    fflush(stdout);

    test_processpool_stop();
}