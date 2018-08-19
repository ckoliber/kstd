typedef struct Lock {
    int lock();
    int unlock();
    int trylock(int timeout);
} Lock;

Lock* lock_new();
void lock_free(Lock* lock);