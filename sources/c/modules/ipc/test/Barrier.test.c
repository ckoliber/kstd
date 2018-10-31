#include <low/Barrier.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>

#if defined(APP_WINDOWS)
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#define sleep(x) sleep(x)
#endif

int function_1(void* arg){
    Barrier* barrier = arg;

    for(int a = 0 ; a < 5 ; a++){
        assert(barrier->vtable->wait(barrier, 500) == -1);

        assert(barrier->vtable->wait(barrier, UINT_64_MAX) == 0);
    }

    return 0;
}

int function_2(void* arg){
    Barrier* barrier = arg;

    for(int a = 0 ; a < 5 ; a++){
        sleep(1);

        assert(barrier->vtable->wait(barrier, UINT_64_MAX) == 0);
    }

    return 0;
}

void test_barrier_wait();
void test_barrier_get();

void test_barrier_wait(){
    Barrier* barrier = barrier_new_anonymous(2);

    Thread* t1 = thread_new_object(0);

    Thread* t2 = thread_new_object(0);

    t1->vtable->start(t1, function_1, barrier);

    t2->vtable->start(t2, function_2, barrier);

    t1->vtable->join(t1);

    t2->vtable->join(t2);

    thread_free(t1);

    thread_free(t2);

    barrier_free(barrier);
}
void test_barrier_get(){
    Barrier* barrier = barrier_new_anonymous(2);

    assert(barrier->vtable->get(barrier) == 0);

    barrier_free(barrier);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 2 ; a++){
        test_barrier_wait();
        test_barrier_get();
    }
}

