typedef struct FD {
    int openfile(char* path);
    int opensocket(char* host, int port);
    struct FD* accept();
    int read(void* data, int size);
    int write(void* data, int size);
    int control(int nonblock);
    int close();
    int file(char* path);
    int socket(char* host, int* port);
} FD;

struct FD* fd_new();
void fd_free(struct FD* fd);