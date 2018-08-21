typedef struct Cond {
    int (*wait)(struct Cond* self, int (*function)(void* arg), void* arg);
    int (*timewait)(struct Cond* self, int (*function)(void* arg), void* arg, int timeout);
    int (*signal)(struct Cond* self, int (*function)(void* arg), void* arg);
    int (*broadcast)(struct Cond* self, int (*function)(void* arg), void* arg);
} Cond;

struct Cond* cond_new();
void cond_free(struct Cond* cond);