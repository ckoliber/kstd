#include <kstd.h>
#include <low/ReadWriteLock.h>
#include <low/Thread.h>

#include <assert.h>
#include <unistd.h>
#include <stdio.h>

int function(uint_8* arg) {
    ReadWriteLock* lock = (ReadWriteLock *) arg;

    assert(lock->vtable->read_lock(lock, 0) == 0);

    assert(lock->vtable->write_lock(lock, 500) == -1);

    assert(lock->vtable->read_unlock(lock) == 0);

    assert(lock->vtable->write_lock(lock, UINT_64_MAX) == 0);

    assert(lock->vtable->write_unlock(lock) == 0);

    return 0;
}

void test_readwritelock();

void test_readwritelock() {
    ReadWriteLock* lock = readwritelock_new_object(NULL);

    assert(lock->vtable->read_lock(lock, 0) == 0);

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, (uint_8*) lock);

    sleep(1);

    assert(lock->vtable->read_unlock(lock) == 0);

    t->vtable->join(t);

    thread_free(t);

    readwritelock_free(lock);
}

int main() {
    kstd_init();

    test_readwritelock();

}