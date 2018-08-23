typedef struct Cond {
    int (*wait)(struct Cond* self, int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2);
    int (*timewait)(struct Cond* self, int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2, long int timeout);
    int (*signal)(struct Cond* self, void (*critical)(void*), void* arg);
    int (*broadcast)(struct Cond* self, void (*critical)(void*), void* arg);
} Cond;

struct Cond* cond_new();
void cond_free(struct Cond* cond);