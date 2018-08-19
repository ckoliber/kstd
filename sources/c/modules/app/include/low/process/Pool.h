typedef struct Pool {
    int start();
    int post(void (*function)(void* arg), void* arg);
    int stop();
} Pool;

struct Pool* pool_new(int size);
void pool_free(struct Pool* pool);