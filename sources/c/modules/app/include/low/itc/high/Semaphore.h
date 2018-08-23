typedef struct Semaphore {
    int (*wait)(struct Semaphore* self, int count);
    int (*timewait)(struct Semaphore* self, int count, long int timeout);
    int (*post)(struct Semaphore* self, int count);
    int (*get)(struct Semaphore* self);
} Semaphore;

struct Semaphore* semaphore_new(int value);
void semaphore_free(struct Semaphore* semaphore);