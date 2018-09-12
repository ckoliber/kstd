#include <ipc/high/Lock.h>

#include <dsa/low/String.h>
#include <ipc/low/Mutex.h>
#include <memory/low/Heap.h>

struct Lock_ {
    struct Lock self;
    Mutex* mutex;
};

// link methods
int lock_lock(struct Lock* self, uint_64 timeout);
int lock_unlock(struct Lock* self);

// implement methods
int lock_lock(struct Lock* self, uint_64 timeout) {
    struct Lock_* lock_ = (struct Lock_*)self;

    // acquire internal Mutex
    int result = lock_->mutex->acquire(lock_->mutex, timeout);

    return result;
}
int lock_unlock(struct Lock* self) {
    struct Lock_* lock_ = (struct Lock_*)self;

    // release internal Mutex
    int result = lock_->mutex->release(lock_->mutex);

    return result;
}

Lock* lock_new(char* name) {
    struct Lock_* lock_ = heap_alloc(sizeof(struct Lock_));

    // init private methods
    lock_->self.lock = lock_lock;
    lock_->self.unlock = lock_unlock;

    // create internal mutex
    String* lockmutex_name = string_new_concat(name, "/lockmutex");
    lock_->mutex = mutex_new(lockmutex_name->value(lockmutex_name));
    string_free(lockmutex_name);

    return (Lock*)lock_;
}
void lock_free(Lock* lock) {
    struct Lock_* lock_ = (struct Lock_*)lock;

    // destroy internal mutex
    mutex_free(lock_->mutex);

    heap_free(lock_);
}