#include <low/ErrorCheckLock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(uint_8* arg){
    ErrorCheckLock* lock = (ErrorCheckLock *) arg;

    assert(lock->vtable->lock(lock, 500) == -1);

    assert(lock->vtable->lock(lock, UINT_64_MAX) == 0);

    return 0;
}

void test_errorchecklock_lock();
void test_errorchecklock_unlock();

void test_errorchecklock_lock(){
    ErrorCheckLock* lock = errorchecklock_new_object(NULL);

    assert(lock->vtable->lock(lock, 0) == 0);

    assert(lock->vtable->lock(lock, 0) == -1);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    lock->vtable->unlock(lock);

    t->vtable->join(t);

    thread_free(t);

    errorchecklock_free(lock);
}
void test_errorchecklock_unlock(){
    ErrorCheckLock* lock = errorchecklock_new_object(NULL);

    lock->vtable->lock(lock, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    assert(lock->vtable->unlock(lock) == 0);

    assert(lock->vtable->unlock(lock) == -1);

    t->vtable->join(t);

    thread_free(t);

    errorchecklock_free(lock);
}

int main() {
    kstd_init();

    test_errorchecklock_lock();
    test_errorchecklock_unlock();
}