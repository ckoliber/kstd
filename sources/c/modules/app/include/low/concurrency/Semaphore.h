typedef struct Semaphore {
    int (*init)(struct Semaphore* self, int count);
    int (*wait)(struct Semaphore* self, int count);
    int (*post)(struct Semaphore* self, int count);
    int (*get)(struct Semaphore* self);
} Semaphore;

struct Semaphore* semaphore_new();
void semaphore_free(struct Semaphore* semaphore);