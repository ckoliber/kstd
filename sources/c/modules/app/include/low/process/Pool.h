typedef struct Pool {
    int (*start)(struct Pool* self);
    int (*post)(struct Pool* self, void (*function)(void* arg), void* arg);
    int (*stop)(struct Pool* self);
} Pool;

struct Pool* pool_new(int size);
void pool_free(struct Pool* pool);