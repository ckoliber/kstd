#include <low/Process.h>

#if defined(APP_WINDOWS)

#include <low/Heap.h>
#include "ForkProcess.windows.c"
#include "GetParentProcessId.windows.c"

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
int process_start(Process* self, int (*function)(void*), void* arg);
int process_join(Process* self);
int process_id(Process* self);
int process_stop(Process* self);

// implement methods
// vtable operators
int process_start(Process* self, int (*function)(void*), void* arg) {
    struct Process_* process_ = (struct Process_*)self;

    // start internal child process
    process_->id = ForkProcess();
    if (process_->id == 0) {
        // at child process, sucessful fork
        ExitProcess(function(arg));
    } else if (process_->id > 0) {
        // at parent process, sucessful fork
        return 0;
    }

    return -1;
}
int process_join(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // join internal child process
    // open process by id
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_->id);
    if (process == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // wait for process to exit
    if (WaitForSingleObject(process, INFINITE) != WAIT_OBJECT_0) {
        CloseHandle(process);
        return -1;
    }

    // get process exit code
    int result = -1;
    if (GetExitCodeProcess(process, &result) == 0) {
        result = -1;
    }

    // close process handle
    CloseHandle(process);

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
    // open process by id
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_->id);
    if (process == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // terminate process
    if (TerminateProcess(process, -1) == 0) {
        CloseHandle(process);
        return -1;
    }

    // close process handle
    CloseHandle(process);

    return 0;
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

// local process methods
int process_get_self() {
    // get self process id
    int result = (int)GetCurrentProcessId();

    return result;
}
int process_get_parent() {
    // get parent process id
    int result = (int)GetParentProcessId();

    return result;
}

#endif