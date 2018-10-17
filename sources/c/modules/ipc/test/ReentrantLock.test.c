#include <low/ReentrantLock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    ReentrantLock* lock = arg;

    assert(lock->vtable->lock(lock, 3000) == -1);

    assert(lock->vtable->lock(lock, UINT_64_MAX) == 0);

    return 0;
}

void test_reentrantlock_lock();
void test_reentrantlock_unlock();

void test_reentrantlock_lock(){
    ReentrantLock* lock = reentrantlock_new_object(NULL);

    assert(lock->vtable->lock(lock, 0) == 0);

    assert(lock->vtable->lock(lock, 0) == 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, lock);

    sleep(5);

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

    t->vtable->start(t, function, lock);

    sleep(5);

    assert(lock->vtable->unlock(lock) == 0);

    assert(lock->vtable->unlock(lock) == -1);

    t->vtable->join(t);

    thread_free(t);

    reentrantlock_free(lock);
}

int main() {
    kstd_init();

    test_reentrantlock_lock();
    test_reentrantlock_unlock();
}