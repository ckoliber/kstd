typedef struct Semaphore {
    int (*wait)(struct Semaphore* self);
    int (*timewait)(struct Semaphore* self, int timeout);
    int (*post)(struct Semaphore* self);
} Semaphore;

struct Semaphore* semaphore_new(int value);
void semaphore_free(struct Semaphore* semaphore);