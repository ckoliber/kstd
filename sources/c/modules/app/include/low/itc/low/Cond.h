typedef struct Cond {
    int (*wait)(struct Cond* self);
    int (*timewait)(struct Cond* self, int timeout);
    int (*signal)(struct Cond* self);
    int (*broadcast)(struct Cond* self);
} Cond;

struct Cond* cond_new();
void cond_free(struct Cond* cond);