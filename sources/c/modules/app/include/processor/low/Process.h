#include <memory/low/Type.h>

typedef struct Process {
    int (*start)(struct Process* self, int (*function)(void*), void* arg);
    int (*join)(struct Process* self);
    int (*id)(struct Process* self);
    int (*stop)(struct Process* self);
} Process;

Process* process_new();
void process_free(Process* process);

// get self id or parent id
int process_self();
int process_parent();