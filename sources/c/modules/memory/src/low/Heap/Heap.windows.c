#include <low/Heap.h>

#if defined(APP_WINDOWS)

void* heap_alloc(tsize size) {
    void* result = NULL;
    while (result == NULL) {
        result = HeapAlloc(GetProcessHeap(), 0, size);
    }
    return result;
}
void* heap_realloc(void* address, tsize size) {
    void* result = NULL;
    while (result == NULL) {
        result = HeapReAlloc(GetProcessHeap(), 0, address, size);
    }
    return result;
}
void heap_free(void* address) {
    if (address != NULL) {
        HeapFree(GetProcessHeap(), 0, address);
    }
}
void heap_copy(void* address, void* source, tsize size) {
    CopyMemory(address, source, size);
}
void heap_move(void* address, void* source, tsize size) {
    MoveMemory(address, source, size);
}
void heap_set(void* address, char source, tsize size) {
    FillMemory(address, size, source);
}

#endif