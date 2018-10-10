#include <low/Mutex.h>

#include <assert.h>

int main() {
    Mutex* mutex = mutex_new_object(0, NULL);

    mutex_free(mutex);
}