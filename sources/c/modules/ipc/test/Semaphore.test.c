#include <low/Semaphore.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    Semaphore* semaphore = arg;

    assert(semaphore->vtable->wait(semaphore, 3000) == -1);

    assert(semaphore->vtable->wait(semaphore, UINT_64_MAX) == 0);

    return 0;
}

int main() {
    kstd_init();

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