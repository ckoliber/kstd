// Thanks by: https://github.com/kaniini/win32-fork

#include <low/Process.h>

#if defined(APP_WINDOWS)

#define _WIN32_WINNT 0x0600
#define WIN32_LEAN_AND_MEAN
#include <errno.h>
#include <ntdef.h>
#include <process.h>
#include <stdio.h>
#include <windows.h>
#include <winnt.h>

#ifdef __MINGW32__
typedef struct _CLIENT_ID {
    PVOID UniqueProcess;
    PVOID UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _SECTION_IMAGE_INFORMATION {
    PVOID EntryPoint;
    ULONG StackZeroBits;
    ULONG StackReserved;
    ULONG StackCommit;
    ULONG ImageSubsystem;
    WORD SubSystemVersionLow;
    WORD SubSystemVersionHigh;
    ULONG Unknown1;
    ULONG ImageCharacteristics;
    ULONG ImageMachineType;
    ULONG Unknown2[3];
} SECTION_IMAGE_INFORMATION, *PSECTION_IMAGE_INFORMATION;

typedef struct _RTL_USER_PROCESS_INFORMATION {
    ULONG Size;
    HANDLE Process;
    HANDLE Thread;
    CLIENT_ID ClientId;
    SECTION_IMAGE_INFORMATION ImageInformation;
} RTL_USER_PROCESS_INFORMATION, *PRTL_USER_PROCESS_INFORMATION;

#define RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED 0x00000001
#define RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES 0x00000002
#define RTL_CLONE_PROCESS_FLAGS_NO_SYNCHRONIZE 0x00000004

#define RTL_CLONE_PARENT 0
#define RTL_CLONE_CHILD 297

#endif

pid_t ForkProcess(void);

typedef NTSTATUS (*RtlCloneUserProcess_f)(ULONG ProcessFlags,
                                          PSECURITY_DESCRIPTOR ProcessSecurityDescriptor /* optional */,
                                          PSECURITY_DESCRIPTOR ThreadSecurityDescriptor /* optional */,
                                          HANDLE DebugPort /* optional */,
                                          PRTL_USER_PROCESS_INFORMATION ProcessInformation);

pid_t ForkProcess(void) {
    HMODULE mod;
    RtlCloneUserProcess_f clone_p;
    RTL_USER_PROCESS_INFORMATION process_info;
    NTSTATUS result;

    mod = GetModuleHandle("ntdll.dll");
    if (!mod)
        return -ENOSYS;

    clone_p = GetProcAddress(mod, "RtlCloneUserProcess");
    if (clone_p == NULL)
        return -ENOSYS;

    /* lets do this */
    result = clone_p(RTL_CLONE_PROCESS_FLAGS_CREATE_SUSPENDED | RTL_CLONE_PROCESS_FLAGS_INHERIT_HANDLES, NULL, NULL, NULL, &process_info);

    if (result == RTL_CLONE_PARENT) {
        HANDLE me = GetCurrentProcess();
        pid_t child_pid;

        child_pid = GetProcessId(process_info.Process);

        ResumeThread(process_info.Thread);
        CloseHandle(process_info.Process);
        CloseHandle(process_info.Thread);

        return child_pid;
    } else if (result == RTL_CLONE_CHILD) {
        /* fix stdio */
        AllocConsole();
        return 0;
    } else
        return -1;

    /* NOTREACHED */
    return -1;
}

#endif