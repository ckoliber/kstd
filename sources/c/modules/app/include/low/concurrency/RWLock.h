typedef struct RWLock {
    int (*rlock)(struct RWLock *self);
    int (*wlock)(struct RWLock *self);
    int (*runlock)(struct RWLock *self);
    int (*wunlock)(struct RWLock *self);
    int (*tryrlock)(struct RWLock *self, int timeout);
    int (*trywlock)(struct RWLock *self, int timeout);
} RWLock;

struct RWLock *rwlock_new();
void rwlock_free(struct RWLock *rwlock);