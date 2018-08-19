typedef struct Pool {
    int start();
    int post(void (*function)(void* arg), void* arg);
    int stop();
} Pool;

Pool* pool_new(int size);
void pool_free(Pool* pool);