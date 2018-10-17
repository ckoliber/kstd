#include <low/MutexLock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    MutexLock* mutex = arg;

    assert(mutex->vtable->acquire(mutex, 3000) == -1);

    assert(mutex->vtable->acquire(mutex, UINT_64_MAX) == 0);

    return 0;
}

int main() {
    kstd_init();

    MutexLock* mutex = mutex_new_object(0, NULL);

    assert(mutex->vtable->acquire(mutex, 0) == 0);

    Thread* t = thread_new_object(0);
    t->vtable->start(t, function, mutex);

    sleep(5);
    assert(mutex->vtable->release(mutex) == 0);

    t->vtable->join(t);

    thread_free(t);

    mutex_free(mutex);
}