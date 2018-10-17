#include <low/MutexLock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    MutexLock* lock = arg;

    assert(lock->vtable->lock(lock, 3000) == -1);

    assert(lock->vtable->lock(lock, UINT_64_MAX) == 0);

    return 0;
}

void test_mutexlock_lock();
void test_mutexlock_unlock();

void test_mutexlock_lock(){
    MutexLock* lock = mutexlock_new_object(NULL);

    assert(lock->vtable->lock(lock, 0) == 0);

    assert(lock->vtable->lock(lock, 0) == -1);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, lock);

    sleep(5);

    lock->vtable->unlock(lock);

    t->vtable->join(t);

    thread_free(t);

    mutexlock_free(lock);
}
void test_mutexlock_unlock(){
    MutexLock* lock = mutexlock_new_object(NULL);

    lock->vtable->lock(lock, 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, lock);

    sleep(5);

    assert(lock->vtable->unlock(lock) == 0);

    assert(lock->vtable->unlock(lock) == 0);

    t->vtable->join(t);

    thread_free(t);

    mutexlock_free(lock);
}

int main() {
    kstd_init();

    test_mutexlock_lock();
    test_mutexlock_unlock();
}