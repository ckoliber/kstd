typedef struct RWLock {
    int rlock();
    int wlock();
    int runlock();
    int wunlock();
    int tryrlock(int timeout);
    int trywlock(int timeout);
} RWLock;

struct RWLock* rwlock_new();
void rwlock_free(struct RWLock* rwlock);