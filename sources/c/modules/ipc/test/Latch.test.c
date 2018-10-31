#include <low/Latch.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>

#if defined(APP_WINDOWS)
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#define sleep(x) sleep(x)
#endif

int function(void* arg){
    Latch* latch = arg;

    for(int a = 0 ; a < 5 ; a++){
        sleep(1);

        latch->vtable->down(latch);
    }

    return 0;
}

void test_latch_wait();
void test_latch_down();
void test_latch_get();

void test_latch_wait(){
    Latch* latch = latch_new_anonymous(1);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, latch);

    for(int a = 0 ; a < 5 ; a++){
        assert(latch->vtable->wait(latch, 500) == -1);

        assert(latch->vtable->wait(latch, UINT_64_MAX) == 0);
    }

    t->vtable->join(t);

    thread_free(t);

    latch_free(latch);
}
void test_latch_down(){
    Latch* latch = latch_new_anonymous(1);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, latch);

    for(int a = 0 ; a < 5 ; a++){
        assert(latch->vtable->wait(latch, 500) == -1);

        assert(latch->vtable->wait(latch, UINT_64_MAX) == 0);
    }

    t->vtable->join(t);

    thread_free(t);

    latch_free(latch);
}
void test_latch_get(){
    Latch* latch = latch_new_anonymous(1);

    assert(latch->vtable->get(latch) == 0);

    latch_free(latch);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 2 ; a++){
        test_latch_wait();
        test_latch_down();
        test_latch_get();
    }
}

