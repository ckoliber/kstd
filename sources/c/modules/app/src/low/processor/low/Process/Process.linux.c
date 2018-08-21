#ifdef __unix__

#include <low/processor/low/Process.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct Process_ {
    struct Process self;
    pid_t id;
};

// link methods
int process_start(struct Process* self, char* command, char* argv[]);
int process_join(struct Process* self);
int process_id(struct Process* self);
int process_stop(struct Process* self);

int process_start(struct Process* self, char* command, char* argv[]) {
    struct Process_* process_ = self;

    // start internal process
    int result = 0;
    process_->id = fork();
    if (process_->id == 0) {
        execv(command, argv);
        exit(0);
    } else if (process_->id == -1) {
        result = -1;
    }

    return 0;
}
int process_join(struct Process* self) {
    struct Process_* process_ = self;

    // join internal process
    int result = 0;
    waitpid(process_->id, &result, 0);

    return result;
}
int process_id(struct Process* self) {
    struct Process_* process_ = self;

    // get internal process id
    int result = process_->id;

    return result;
}
int process_stop(struct Process* self) {
    struct Process_* process_ = self;

    // stop internal process
    kill(process_->id, SIGKILL);

    return 0;
}

struct Process* process_new() {
    struct Process_* process_ = memory_alloc(sizeof(struct Process_));

    // init private methods
    process_->self.start = process_start;
    process_->self.join = process_join;
    process_->self.id = process_id;
    process_->self.stop = process_stop;

    // init internal process

    return process_;
}
void process_free(struct Process* process) {
    struct Process_* process_ = process;

    // destry internal process

    memory_free(process_);
}

#endif