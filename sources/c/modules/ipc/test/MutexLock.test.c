#include <low/MutexLock.h>
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
    MutexLock* lock = (MutexLock *) arg;

    assert(lock->vtable->lock(lock, 500) == -1);

    assert(lock->vtable->lock(lock, UINT_64_MAX) == 0);

    assert(lock->vtable->unlock(lock) == 0);

    return 0;
}

void test_mutexlock_lock();
void test_mutexlock_unlock();

void test_mutexlock_lock(){
    MutexLock* lock = mutexlock_new_object(NULL);

    assert(lock->vtable->lock(lock, 0) == 0);

    assert(lock->vtable->lock(lock, 0) == -1);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    lock->vtable->unlock(lock);

    t->vtable->join(t);

    thread_free(t);

    mutexlock_free(lock);
}
void test_mutexlock_unlock(){
    MutexLock* lock = mutexlock_new_object(NULL);

    lock->vtable->lock(lock, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    assert(lock->vtable->unlock(lock) == 0);

    t->vtable->join(t);

    assert(lock->vtable->unlock(lock) == 0);

    thread_free(t);

    mutexlock_free(lock);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_mutexlock_lock();
        test_mutexlock_unlock();
    }
}