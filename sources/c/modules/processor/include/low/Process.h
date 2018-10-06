#include <memory/low/Type.h>

// vtable
typedef struct Process_VTable {
    int (*start)(struct Process* self, int (*function)(void*), void* arg);
    int (*join)(struct Process* self);
    int (*id)(struct Process* self);
    int (*stop)(struct Process* self);
} Process_VTable;

// vtable + private data problem solve
typedef struct Process {
    Process_VTable* vtable;
} Process;

// init vtable
void process_init();

// new raw process
Process* process_new();

// free raw thread
void process_free(Process* process);

// new process
Process* process_new_object();

// get self id or parent id
int process_self();
int process_parent();