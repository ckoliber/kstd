typedef struct RWLock {
    int rlock();
    int wlock();
    int runlock();
    int wunlock();
    int tryrlock(int timeout);
    int trywlock(int timeout);
} RWLock;

RWLock* rwlock_new();
void rwlock_free(RWLock* rwlock);