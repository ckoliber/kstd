typedef struct RWLock {
    int (*readlock)(struct RWLock* self);
    int (*writelock)(struct RWLock* self);
    int (*timereadlock)(struct RWLock* self, long int timeout);
    int (*timewritelock)(struct RWLock* self, long int timeout);
    int (*readunlock)(struct RWLock* self);
    int (*writeunlock)(struct RWLock* self);
} RWLock;

struct RWLock* rwlock_new();
void rwlock_free(struct RWLock* rwlock);