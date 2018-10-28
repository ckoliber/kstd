#include <low/Semaphore.h>

#if defined(APP_WINDOWS)

// nt query semaphore for getting value
typedef LONG NTSTATUS;

typedef NTSTATUS (NTAPI *_NtQuerySemaphore)(
        HANDLE SemaphoreHandle,
        DWORD SemaphoreInformationClass,
        PVOID SemaphoreInformation,
        ULONG SemaphoreInformationLength,
        PULONG ReturnLength OPTIONAL
);

typedef struct _SEMAPHORE_BASIC_INFORMATION {
    ULONG CurrentCount;
    ULONG MaximumCount;
} SEMAPHORE_BASIC_INFORMATION;

#include <low/Heap.h>
#include <low/String.h>

struct Semaphore_ {
    // self public object
    Semaphore self;

    // constructor data

    // private data
    HANDLE semaphore_handle;
};

// vtable
Semaphore_VTable* semaphore_vtable;

// link methods
int semaphore_wait(Semaphore* self, uint_64 timeout);
int semaphore_post(Semaphore* self);
int semaphore_get(Semaphore* self);

// implement methods
// vtable operators
int semaphore_wait(Semaphore* self, uint_64 timeout) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // wait the wi32 semaphore
    if (timeout == UINT_64_MAX) {

        // infinity
        if (WaitForSingleObject(semaphore_->semaphore_handle, INFINITE) == WAIT_OBJECT_0) {
            return 0;
        }

    } else {
        // timed, try
        if (WaitForSingleObject(semaphore_->semaphore_handle, (DWORD) timeout) == WAIT_OBJECT_0) {
            return 0;
        }
    }

    return -1;
}
int semaphore_post(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // release the win32 semaphore
    if (ReleaseSemaphore(semaphore_->semaphore_handle, 1, NULL) != 0) {
        return 0;
    }

    return -1;
}
int semaphore_get(Semaphore* self) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)self;

    // get nt query semaphore proc from nt kernel
    _NtQuerySemaphore NtQuerySemaphore = (_NtQuerySemaphore)GetProcAddress (GetModuleHandle ("ntdll.dll"), "NtQuerySemaphore");
    if (NtQuerySemaphore) {
        SEMAPHORE_BASIC_INFORMATION BasicInfo;
        if (NtQuerySemaphore(semaphore_->semaphore_handle, 0, &BasicInfo, sizeof (SEMAPHORE_BASIC_INFORMATION), NULL) == ERROR_SUCCESS) {
            return (int) BasicInfo.CurrentCount;
        }
    }

    return -1;
}

// object allocation and deallocation operators
void semaphore_init() {
    // init vtable
    semaphore_vtable = heap_alloc(sizeof(Semaphore_VTable));
    semaphore_vtable->wait = semaphore_wait;
    semaphore_vtable->post = semaphore_post;
    semaphore_vtable->get = semaphore_get;
}
Semaphore* semaphore_new() {
    struct Semaphore_* semaphore_ = heap_alloc(sizeof(struct Semaphore_));

    // set vtable
    semaphore_->self.vtable = semaphore_vtable;

    // set constructor data

    // set private data
    semaphore_->semaphore_handle = INVALID_HANDLE_VALUE;

    return (Semaphore*)semaphore_;
}
void semaphore_free(Semaphore* semaphore) {
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore;

    // free private data
    if (semaphore_->semaphore_handle != INVALID_HANDLE_VALUE) {
        CloseHandle(semaphore_->semaphore_handle);
    }

    // free constructor data

    // free self
    heap_free(semaphore_);
}
Semaphore* semaphore_new_anonymous(int value){
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data

    // set private data
    semaphore_->semaphore_handle = CreateSemaphoreA(
            NULL,
            value,
            UINT_16_MAX,
            NULL);

    return (Semaphore*)semaphore_;
}
Semaphore* semaphore_new_named(char* name, int value){
    struct Semaphore_* semaphore_ = (struct Semaphore_*)semaphore_new();

    // set constructor data

    // set private data
    String* semaphore_name = string_new_printf("%s_sm", name);
    semaphore_->semaphore_handle = CreateSemaphoreA(
            NULL,
            value,
            UINT_16_MAX,
            semaphore_name->vtable->value(semaphore_name));
    string_free(semaphore_name);

    return (Semaphore*)semaphore_;
}

#endif
