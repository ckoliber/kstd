#include <processor/low/Process.h>

#if defined(APP_LINUX)

#include <memory/low/Heap.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct Process_ {
    struct Process self;
    pid_t id;
};

// link methods
int process_priority(struct Process* self, int priority);
int process_affinity(struct Process* self, int affinity);
int process_start(struct Process* self, int (*function)(void*), void* arg);
int process_join(struct Process* self);
int process_id(struct Process* self);
int process_stop(struct Process* self);

// implement methods
int process_priority(struct Process* self, int priority) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // set priority
    int result = -1;

    return result;
}
int process_affinity(struct Process* self, int affinity) {
    struct Thread_* thread_ = (struct Thread_*)self;

    // set affinity
    int result = -1;

    return result;
}
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

Process* process_new() {
    struct Process_* process_ = heap_alloc(sizeof(struct Process_));

    // init private methods
    process_->self.priority = process_priority;
    process_->self.affinity = process_affinity;
    process_->self.start = process_start;
    process_->self.join = process_join;
    process_->self.id = process_id;
    process_->self.stop = process_stop;

    return (Process*)process_;
}
void process_free(Process* process) {
    struct Process_* process_ = (struct Process_*)process;

    heap_free(process_);
}

#endif