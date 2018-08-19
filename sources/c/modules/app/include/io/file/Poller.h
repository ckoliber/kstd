typedef struct Poller {
    int add(struct FD* fd);
    int remove(struct FD* fd);
    int start();
    int stop();
} Poller;

struct Poller* poller_new(void (*onAccept)(struct FD* fd), void (*onClose)(struct FD* fd), void (*onRead)(struct FD* fd, void* data, int size));
void poller_free(struct Poller* poller);