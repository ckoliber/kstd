typedef struct Lock {
    int (*lock)(struct Lock* self);
    int (*timelock)(struct Lock* self, long int timeout);
    int (*unlock)(struct Lock* self);
} Lock;

struct Lock* lock_new();
void lock_free(struct Lock* lock);