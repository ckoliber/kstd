typedef struct Mutex {
    int (*lock)(struct Mutex* self);
    int (*timelock)(struct Mutex* self, long int timeout);
    int (*unlock)(struct Mutex* self);
} Mutex;

struct Mutex* mutex_new();
void mutex_free(struct Mutex* mutex);