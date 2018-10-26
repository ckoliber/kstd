#include <low/Semaphore.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>

#if defined(APP_WINDOWS)
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#define sleep(x) sleep(x)
#endif

int function(uint_8* arg){
    Semaphore* semaphore = (Semaphore *) arg;

    assert(semaphore->vtable->wait(semaphore, 500) == -1);

    assert(semaphore->vtable->wait(semaphore, UINT_64_MAX) == 0);

    return 0;
}

void test_semaphore_wait();
void test_semaphore_post();
void test_semaphore_get();

void test_semaphore_wait(){
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) semaphore);

    sleep(1);

    semaphore->vtable->post(semaphore);

    t->vtable->join(t);

    thread_free(t);

    semaphore_free(semaphore);
}
void test_semaphore_post(){
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) semaphore);

    sleep(1);

    assert(semaphore->vtable->post(semaphore) == 0);

    t->vtable->join(t);

    thread_free(t);

    semaphore_free(semaphore);
}
void test_semaphore_get(){
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) semaphore);

    sleep(1);

    assert(semaphore->vtable->post(semaphore) == 0);

    t->vtable->join(t);

    assert(semaphore->vtable->get(semaphore) == 0);

    thread_free(t);

    semaphore_free(semaphore);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_semaphore_wait();
        test_semaphore_post();
        test_semaphore_get();
    }
}