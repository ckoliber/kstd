// Thanks by: https://github.com/ambakshi/ppid-win32

// AMB: Print out the parent PID. You can pass in -p <pid> to ask for
// the parent of a particular process
#define _WIN32_WINNT 0x0503
#include <stdio.h>
#include <windows.h>

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