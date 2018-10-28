#include <low/Process.h>

#if defined(APP_WINDOWS) && defined(APP_WINDOWS_MSVC)

// Thanks by: https://github.com/ambakshi/ppid-win32
ULONG_PTR GetParentProcessId(){
    ULONG_PTR pbi[6];
    ULONG ulSize = 0;
    LONG (WINAPI *NtQueryInformationProcess)(HANDLE ProcessHandle, ULONG ProcessInformationClass,
                                             PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);
    *(FARPROC *)&NtQueryInformationProcess =
            GetProcAddress(LoadLibraryA("NTDLL.DLL"), "NtQueryInformationProcess");
    if(NtQueryInformationProcess){
        if(NtQueryInformationProcess(GetCurrentProcess(), 0,
                                     &pbi, sizeof(pbi), &ulSize) >= 0 && ulSize == sizeof(pbi))
            return pbi[5];
    }
    return (ULONG_PTR)-1;
}

// Thanks by: https://github.com/kaniini/win32-fork
DWORD ForkProcess(void) {
    return (DWORD) -1;
}

// Process.windows.c body
#include <kstd.h>
#include <low/Heap.h>

struct Process_ {
    // self public object
    Process self;

    // constructor data

    // private data
    HANDLE process_handle;
};

// vtable
Process_VTable* process_vtable;

// link methods
int process_start(Process* self, int (*function)(void*), void* arg);
int process_join(Process* self);
uint_64 process_id(Process* self);
int process_stop(Process* self);

// implement methods
// vtable operators
int process_start(Process* self, int (*function)(void*), void* arg) {
    struct Process_* process_ = (struct Process_*)self;

    // start internal child process
    DWORD child_id = ForkProcess();
    if (child_id == 0) {
        // at child process, sucessful fork
        kstd_init_child();
        ExitProcess((DWORD)function(arg));
    } else if (child_id > 0) {
        // at parent process, sucessful fork
        process_->process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, child_id);
        return 0;
    }

    return -1;
}
int process_join(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // join internal child process
    // wait for process to exit
    if (WaitForSingleObject(process_->process_handle, INFINITE) != WAIT_OBJECT_0) {
        return -1;
    }

    // get process exit code
    int result = -1;
    if (GetExitCodeProcess(process_->process_handle, (PDWORD)&result) == 0) {
        result = -1;
    }

    return result;
}
uint_64 process_id(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // get internal child process id
    uint_64 result = GetProcessId(process_->process_handle);

    return result;
}
int process_stop(Process* self) {
    struct Process_* process_ = (struct Process_*)self;

    // stop internal child process
    // terminate process
    if (TerminateProcess(process_->process_handle, (DWORD)-1) == 0) {
        return -1;
    }

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
    process_->process_handle = INVALID_HANDLE_VALUE;

    return (Process*)process_;
}
void process_free(Process* process) {
    struct Process_* process_ = (struct Process_*)process;

    // free private data
    if(process_->process_handle != INVALID_HANDLE_VALUE){
        CloseHandle(process_->process_handle);
    }

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
