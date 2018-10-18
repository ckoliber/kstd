#include <low/Process.h>

#if defined(APP_WINDOWS)

// Thanks by: https://github.com/ambakshi/ppid-win32

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#ifndef PROCESS_QUERY_LIMITED_INFORMATION
#define PROCESS_QUERY_LIMITED_INFORMATION 0x1000
#endif

LONG(WINAPI *NtQueryInformationProcess)
        (HANDLE ProcessHandle, ULONG ProcessInformationClass,
         PVOID ProcessInformation, ULONG ProcessInformationLength, PULONG ReturnLength);

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    ULONG AllocationSize;
    ULONG Size;
    ULONG Flags;
    ULONG DebugFlags;
    HANDLE hConsole;
    ULONG ProcessGroup;
    HANDLE hStdInput;
    HANDLE hStdOutput;
    HANDLE hStdError;
    UNICODE_STRING CurrentDirectoryName;
    HANDLE CurrentDirectoryHandle;
    UNICODE_STRING DllPath;
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
    PWSTR Environment;
    ULONG dwX;
    ULONG dwY;
    ULONG dwXSize;
    ULONG dwYSize;
    ULONG dwXCountChars;
    ULONG dwYCountChars;
    ULONG dwFillAttribute;
    ULONG dwFlags;
    ULONG wShowWindow;
    UNICODE_STRING WindowTitle;
    UNICODE_STRING DesktopInfo;
    UNICODE_STRING ShellInfo;
    UNICODE_STRING RuntimeInfo;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef void *PPEB_LDR_DATA;
typedef void *PPS_POST_PROCESS_INIT_ROUTINE;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    BYTE Reserved4[104];
    PVOID Reserved5[52];
    PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
    BYTE Reserved6[128];
    PVOID Reserved7[1];
    ULONG SessionId;
} PEB, *PPEB;

struct PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
};

DWORD GetParentProcessId() {
    struct PROCESS_BASIC_INFORMATION pbi;
    ULONG ulSize = 0;
    if (NtQueryInformationProcess) {
        if (NtQueryInformationProcess(GetCurrentProcess(), 0, &pbi, sizeof(pbi), &ulSize) >= 0 && ulSize == sizeof(pbi)) {
            return (DWORD)pbi.Reserved3;
        }
    }
    return -1;
}







// Thanks by: https://github.com/kaniini/win32-fork

//#include <errno.h>
//#include <ntdef.h>
#include <process.h>
//
//typedef struct _RTL_USER_PROCESS_INFORMATION {
//    ULONG Size;
//    HANDLE Process;
//    HANDLE Thread;
//} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;
//
//#define RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED 0x00000001
//#define RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES 0x00000002
//
//#define RTL_CLONE_PARENT 0
//#define RTL_CLONE_CHILD 297
//
pid_t ForkProcess(void);
//
//typedef NTSTATUS (*RtlCloneUserProcess_f)(ULONG ProcessFlags,
//                                          PSECURITY_DESCRIPTOR ProcessSecurityDescriptor /* optional */,
//                                          PSECURITY_DESCRIPTOR ThreadSecurityDescriptor /* optional */,
//                                          HANDLE DebugPort /* optional */,
//                                          PRTL_USER_PROCESS_INFORMATION ProcessInformation);

//pid_t ForkProcess(void) {
//    HMODULE mod;
//    RtlCloneUserProcess_f clone_p;
//    RTL_USER_PROCESS_INFORMATION process_info;
//    NTSTATUS result;
//
//    mod = GetModuleHandle("ntdll.dll");
//    if (!mod)
//        return -ENOSYS;
//
//    clone_p = (RtlCloneUserProcess_f) GetProcAddress(mod, "RtlCloneUserProcess");
//    if (clone_p == NULL)
//        return -ENOSYS;
//
//    result = clone_p(RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED | RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES, NULL, NULL, NULL, &process_info);
//
//    if (result == RTL_CLONE_PARENT) {
//        pid_t child_pid = -1;
//
////        child_pid = GetProcessId(process_info.Process);
//
//        ResumeThread(process_info.Thread);
//        CloseHandle(process_info.Process);
//        CloseHandle(process_info.Thread);
//
//        return child_pid;
//    } else if (result == RTL_CLONE_CHILD) {
//        /* fix stdio */
//        AllocConsole();
//        return 0;
//    } else {
//        return -1;
//    }
//}

pid_t ForkProcess(void){
    return -1;
}




// Process.windows.c body

#include <low/Heap.h>
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
    process_->id = ForkProcess();
    if (process_->id == 0) {
        // at child process, sucessful fork
        kstd_init_child();
        ExitProcess((DWORD) function(arg));
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
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD) process_->id);
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
    if (GetExitCodeProcess(process, (PDWORD) &result) == 0) {
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
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD) process_->id);
    if (process == INVALID_HANDLE_VALUE) {
        return -1;
    }

    // terminate process
    if (TerminateProcess(process, (DWORD) -1) == 0) {
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
    int result = (int)GetCurrentProcessId();

    return result;
}
int process_get_parent() {
    // get parent process id
    int result = (int)GetParentProcessId();

    return result;
}

#endif
