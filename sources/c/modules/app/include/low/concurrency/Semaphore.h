typedef struct Semaphore {
    int init(int count);
    int wait(int count);
    int post(int count);
    int get();
} Semaphore;

Semaphore* semaphore_new();
void semaphore_free(Semaphore* semaphore);