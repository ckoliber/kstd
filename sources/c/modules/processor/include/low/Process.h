#include <low/Type.h>

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

// local process methods
<<<<<<< HEAD
int process_id();
=======
int process_self();
>>>>>>> a37be36ed975eeb630021237dedce3819a9faeac
int process_parent();