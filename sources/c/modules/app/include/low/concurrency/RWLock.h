typedef struct RWLock {
    int (*rlock)(struct RWLock *self);
    int (*wlock)(struct RWLock *self);
    int (*runlock)(struct RWLock *self);
    int (*wunlock)(struct RWLock *self);
} RWLock;

struct RWLock *rwlock_new();
void rwlock_free(struct RWLock *rwlock);