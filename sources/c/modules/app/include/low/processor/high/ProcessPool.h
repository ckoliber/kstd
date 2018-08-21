typedef struct ProcessPool {
    int (*start)(struct ProcessPool* self);
    int (*post)(struct ProcessPool* self, char* command);
    int (*stop)(struct ProcessPool* self);
} ProcessPool;

struct ProcessPool* processpool_new(int size);
void processpool_free(struct ProcessPool* processpool);