#include <high/RWLock.h>

#include <assert.h>

int main() {
    RWLock* rwlock = rwlock_new_object(0, NULL);

    rwlock_free(rwlock);
}