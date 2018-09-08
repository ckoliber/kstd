#include <memory/low/Type.h>

typedef struct Process {
    int (*priority)(struct Process* self, int priority);
    int (*affinity)(struct Process* self, int affinity);
    int (*start)(struct Process* self, void* (*function)(void*), void* arg);
    int (*join)(struct Process* self);
    int (*id)(struct Process* self);
    int (*stop)(struct Process* self);
} Process;

Process* process_new();
void process_free(Process* process);