#include <high/ProcessPool.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

struct Message{
    void (*function)(uint_8*);
    int arg;
};

void test_function(uint_8* arg) {
    struct Message* message = (struct Message*) arg;

    assert(message->arg == 4);
}

void test_processpool_start();
void test_processpool_post();
void test_processpool_stop();

void test_processpool_start() {
    ProcessPool* processpool = processpool_new_object(2, sizeof(struct Message));

    assert(processpool->vtable->start(processpool) == 0);

    processpool->vtable->stop(processpool);

    processpool_free(processpool);
}
void test_processpool_post() {
    ProcessPool* processpool = processpool_new_object(1, sizeof(struct Message));

    processpool->vtable->start(processpool);

    for (int a = 0; a < 2000; a++) {
        struct Message message = {
                test_function,
                4
        };
        assert(processpool->vtable->post(processpool, (uint_8*) &message) == 0);
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

    for(int a = 0 ; a < 10 ; a++){
        test_processpool_start();
        test_processpool_post();
        test_processpool_stop();
    }
}