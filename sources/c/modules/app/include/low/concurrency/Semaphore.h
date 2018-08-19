typedef struct Semaphore {
    int init(int count);
    int wait(int count);
    int post(int count);
    int get();
} Semaphore;

struct Semaphore* semaphore_new();
void semaphore_free(struct Semaphore* semaphore);