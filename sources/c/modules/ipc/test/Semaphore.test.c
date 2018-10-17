#include <low/Semaphore.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    Semaphore* semaphore = arg;

    assert(semaphore->vtable->wait(semaphore, 1000) == -1);

    assert(semaphore->vtable->wait(semaphore, UINT_64_MAX) == 0);

    return 0;
}

void test_semaphore_wait();
void test_semaphore_post();
void test_semaphore_get();

void test_semaphore_wait(){
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, semaphore);

    sleep(2);

    semaphore->vtable->post(semaphore);

    t->vtable->join(t);

    thread_free(t);

    semaphore_free(semaphore);
}
void test_semaphore_post(){
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, semaphore);

    sleep(5);

    assert(semaphore->vtable->post(semaphore) == 0);

    t->vtable->join(t);

    thread_free(t);

    semaphore_free(semaphore);
}
void test_semaphore_get(){
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, semaphore);

    sleep(5);

    assert(semaphore->vtable->post(semaphore) == 0);

    t->vtable->join(t);

    assert(semaphore->vtable->get(semaphore) == 0);

    thread_free(t);

    semaphore_free(semaphore);
}

int main() {
    kstd_init();

    test_semaphore_wait();
    test_semaphore_post();
    test_semaphore_get();
}