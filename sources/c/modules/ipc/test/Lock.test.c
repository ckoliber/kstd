#include <high/Lock.h>

#include <assert.h>

int main() {
    Lock* lock = lock_new_object(0, NULL);

    lock_free(lock);
}