typedef struct Lock {
    int lock();
    int unlock();
    int trylock(int timeout);
} Lock;

struct Lock* lock_new();
void lock_free(struct Lock* lock);