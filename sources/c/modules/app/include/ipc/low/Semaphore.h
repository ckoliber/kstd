#include <memory/low/Type.h>

typedef struct Semaphore {
    int wait(struct Semaphore* self, uint_64 timeout);
    int post(struct Semaphore* self, int count);
    int get(struct Semaphore* self);
} Semaphore;

Semaphore* semaphore_new(char* name);
void semaphore_free(Semaphore* semaphore);