#include <processor/low/Process.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <memory/low/Heap.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct Process_ {
    // self public object
    Process self;

    // constructor data

    // private data
    pid_t id;
};

// vtable
Process_VTable* process_vtable;

// link methods
int process_start(struct Process* self, int (*function)(void*), void* arg);
int process_join(struct Process* self);
int process_id(struct Process* self);
int process_stop(struct Process* self);

// implement methods
// vtable operators
int process_start(struct Process* self, int (*function)(void*), void* arg) {
    struct Process_* process_ = (struct Process_*)self;

    // start internal child process
    int result = -1;
    process_->id = fork();
    if (process_->id == 0) {
        exit(function(arg));
    } else if (process_->id > 0) {
        result = 0;
    }

    return result;
}
int process_join(struct Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // join internal child process
    int result = -1;
    waitpid(process_->id, &result, 0);

    return result;
}
int process_id(struct Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // get internal child process id
    int result = (int)process_->id;

    return result;
}
int process_stop(struct Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // stop internal child process
    int result = kill(process_->id, SIGKILL);

    return result;
}

// object allocation and deallocation operators
void process_init() {
    // init vtable
    process_vtable = heap_alloc(sizeof(Process_VTable));
    process_vtable->start = process_start;
    process_vtable->join = process_join;
    process_vtable->id = process_id;
    process_vtable->stop = process_stop;
}
Process* process_new() {
    struct Process_* process_ = heap_alloc(sizeof(struct Process_));

    // set vtable
    process_->self.vtable = process_vtable;

    // set constructor data

    // set private data
    process_->id = 0;

    return (Process*)process_;
}
void process_free(Process* process) {
    struct Process_* process_ = (struct Process_*)process;

    // free private data

    // free self
    heap_free(process_);
}
Process* process_new_object() {
    struct Process_* process_ = (struct Process_*)process_new();

    // set constructor data

    // set private data

    return (Process*)process_;
}

// get self id or parent id
int process_self() {
    // get self process id
    int result = (int)getpid();

    return result;
}
int process_parent() {
    // get parent process id
    int result = (int)getppid();

    return result;
}

#endif