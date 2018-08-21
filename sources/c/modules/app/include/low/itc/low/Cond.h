typedef struct Cond {
    int (*wait)(struct Cond* self, int (*condition)(void* arg1), void* arg1, void (*critical)(void* arg2), void* arg2);
    int (*timewait)(struct Cond* self, int (*condition)(void* arg1), void* arg1, void (*critical)(void* arg2), void* arg2, int timeout);
    int (*signal)(struct Cond* self, void (*critical)(void* arg), void* arg);
    int (*broadcast)(struct Cond* self, void (*critical)(void* arg), void* arg);
} Cond;

struct Cond* cond_new();
void cond_free(struct Cond* cond);