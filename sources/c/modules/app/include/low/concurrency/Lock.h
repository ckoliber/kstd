typedef struct Lock {
    int (*lock)(struct Lock* self);
    int (*unlock)(struct Lock* self);
    int (*trylock)(struct Lock* self, int timeout);
} Lock;

struct Lock* lock_new();
void lock_free(struct Lock* lock);