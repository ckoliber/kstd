typedef struct Mutex {
    int (*lock)(struct Mutex* self);
    int (*timelock)(struct Mutex* self, int timeout);
    int (*unlock)(struct Mutex* self);
} Mutex;

struct Mutex* mutex_new();
void mutex_free(struct Mutex* mutex);