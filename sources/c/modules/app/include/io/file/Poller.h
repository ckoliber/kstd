typedef struct Poller {
    int (*add)(struct Poller* self, struct FD* fd);
    int (*remove)(struct Poller* self, struct FD* fd);
    int (*start)(struct Poller* self);
    int (*stop)(struct Poller* self);
} Poller;

struct Poller* poller_new(void (*onAccept)(struct FD* fd), void (*onClose)(struct FD* fd), void (*onRead)(struct FD* fd, void* data, int size));
void poller_free(struct Poller* poller);