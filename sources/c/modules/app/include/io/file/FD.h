typedef struct FD {
    int (*openfile)(struct FD* self, char* path);
    int (*opensocket)(struct FD* self, char* host, int port);
    struct FD* (*accept)(struct FD* self);
    int (*read)(struct FD* self, void* data, int size);
    int (*write)(struct FD* self, void* data, int size);
    int (*control)(struct FD* self, int nonblock);
    int (*close)(struct FD* self);
    int (*file)(struct FD* self, char* path);
    int (*socket)(struct FD* self, char* host, int* port);
} FD;

struct FD* fd_new();
void fd_free(struct FD* fd);