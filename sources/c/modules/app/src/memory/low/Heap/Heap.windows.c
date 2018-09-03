#include <memory/low/Heap.h>

#if defined(APP_WINDOWS)

#include <windows.h>

Void* heap_alloc(Size size) {
    Void* result = NULL;
    while (result == NULL) {
        result = HeapAlloc(GetProcessHeap(), 0, size);
    }
    return result;
}
Void heap_copy(Void* address, Void* source, Size size) {
    CopyMemory(address, source, size);
}
Void heap_move(Void* address, Void* source, Size size) {
    MoveMemory(address, source, size);
}
Void heap_set(Void* address, SignedByte source, Size size) {
    FillMemory(address, size, source);
}
Void* heap_realloc(Void* address, Size size) {
    Void* result = NULL;
    while (result == NULL) {
        result = HeapReAlloc(GetProcessHeap(), 0, address, size);
    }
    return result;
}
Void heap_free(Void* address) {
    if (address != NULL) {
        HeapFree(GetProcessHeap(), 0, address);
    }
}

#endif