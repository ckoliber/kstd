#include <low/Semaphore.h>

#include <assert.h>

int main() {
    Semaphore* semaphore = semaphore_new_object(NULL, 0);

    semaphore_free(semaphore);
}