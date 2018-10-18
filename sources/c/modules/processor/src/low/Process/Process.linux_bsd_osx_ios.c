#include <low/Process.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS)

#include <low/Heap.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <kstd.h>

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
int process_start(Process* self, int (*function)(uint_8*), uint_8* arg);
int process_join(Process* self);
uint_64 process_id(Process* self);
int process_stop(Process* self);

// implement methods
// vtable operators
int process_start(Process* self, int (*function)(uint_8*), uint_8* arg) {
    struct Process_* process_ = (struct Process_*)self;

    // start internal child process
    process_->id = fork();
    if (process_->id == 0) {
        // at child process, sucessful fork
        kstd_init_child();
        exit(function(arg));
    } else if (process_->id > 0) {
        // at parent process, sucessful fork
        return 0;
    }

    return -1;
}
int process_join(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // join internal child process
    int result = -1;
    waitpid(process_->id, &result, 0);
    if(result >= 0){
        result /= 256;
    }

    return result;
}
uint_64 process_id(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // get internal child process id
    uint_64 result = (uint_64)process_->id;

    return result;
}
int process_stop(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // stop internal child process
    if (kill(process_->id, SIGKILL) == 0) {
        return 0;
    }

    return -1;
}

// object allocation and deallocation operators
void process_init() {
    // init vtable
    process_vtable = (Process_VTable*) heap_alloc(sizeof(Process_VTable));
    process_vtable->start = process_start;
    process_vtable->join = process_join;
    process_vtable->id = process_id;
    process_vtable->stop = process_stop;
}
Process* process_new() {
    struct Process_* process_ = (struct Process_*) heap_alloc(sizeof(struct Process_));

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
    heap_free((uint_8*) process_);
}
Process* process_new_object() {
    struct Process_* process_ = (struct Process_*)process_new();

    // set constructor data

    // set private data

    return (Process*)process_;
}

// local process methods
int process_get_self() {
    // get self process id
    int result = (int)getpid();

    return result;
}
int process_get_parent() {
    // get parent process id
    int result = (int)getppid();

    return result;
}

#endif
