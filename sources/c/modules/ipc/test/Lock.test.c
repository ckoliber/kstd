#include <high/Lock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    Lock* lock = arg;

    assert(lock->vtable->lock(lock, 3000) == -1);

    assert(lock->vtable->lock(lock, UINT_64_MAX) == 0);

    return 0;
}

int main() {
    kstd_init();

    Lock* lock = lock_new_object(0, NULL);

    assert(lock->vtable->lock(lock, 0) == 0);

    Thread* t = thread_new_object(0);
    t->vtable->start(t, function, lock);

    sleep(5);
    assert(lock->vtable->unlock(lock) == 0);

    t->vtable->join(t);

    thread_free(t);

    lock_free(lock);
}