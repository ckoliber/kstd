#include <low/Type.h>

// structs
typedef struct Process Process;
typedef struct Process_VTable Process_VTable;

// implement structs
// vtable + private data problem solve
struct Process {
    Process_VTable* vtable;
};

// vtable
struct Process_VTable {
    int (*start)(Process* self, int (*function)(void*), void* arg);
    int (*join)(Process* self);
    int (*id)(Process* self);
    int (*stop)(Process* self);
};

// init vtable
void process_init();

// new raw process
Process* process_new();

// free raw thread
void process_free(Process* process);

// new process
Process* process_new_object();

// local process methods
int process_get_self();
int process_get_parent();