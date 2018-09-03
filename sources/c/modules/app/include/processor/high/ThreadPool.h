typedef struct ThreadPool {
    int (*start)(struct ThreadPool* self);
    int (*post)(struct ThreadPool* self, void (*function)(void*), void* arg);
    int (*stop)(struct ThreadPool* self, int force);
} ThreadPool;

struct ThreadPool* threadpool_new(int size);
void threadpool_free(struct ThreadPool* threadpool);