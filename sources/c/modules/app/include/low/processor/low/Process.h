typedef struct Process {
    int (*start)(struct Process* self, char* command);
    int (*join)(struct Process* self);
    int (*id)(struct Process* self);
    int (*stop)(struct Process* self);
} Process;

struct Process* process_new();
void process_free(struct Process* process);