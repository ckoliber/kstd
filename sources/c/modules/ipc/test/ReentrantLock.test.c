#include <low/ReentrantLock.h>
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
    ReentrantLock* lock = (ReentrantLock *) arg;

    assert(lock->vtable->lock(lock, 500) == -1);

    assert(lock->vtable->lock(lock, UINT_64_MAX) == 0);

    assert(lock->vtable->unlock(lock) == 0);

    return 0;
}

void test_reentrantlock_lock();
void test_reentrantlock_unlock();

void test_reentrantlock_lock(){
    ReentrantLock* lock = reentrantlock_new_object(NULL);

    assert(lock->vtable->lock(lock, 0) == 0);

    assert(lock->vtable->lock(lock, 0) == 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    lock->vtable->unlock(lock);

    lock->vtable->unlock(lock);

    t->vtable->join(t);

    thread_free(t);

    reentrantlock_free(lock);
}
void test_reentrantlock_unlock(){
    ReentrantLock* lock = reentrantlock_new_object(NULL);

    lock->vtable->lock(lock, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    assert(lock->vtable->unlock(lock) == 0);

    t->vtable->join(t);

    assert(lock->vtable->unlock(lock) == -1);

    thread_free(t);

    reentrantlock_free(lock);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_reentrantlock_lock();
        test_reentrantlock_unlock();
    }
}