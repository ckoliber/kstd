typedef struct Semaphore {
    int (*wait)(struct Semaphore* self);
    int (*post)(struct Semaphore* self);
    int (*trywait)(struct Semaphore* self, int timeout);
    int (*get)(struct Semaphore* self);
} Semaphore;

struct Semaphore* semaphore_new(int value);
void semaphore_free(struct Semaphore* semaphore);