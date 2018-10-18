#include <low/Heap.h>

#if defined(APP_WINDOWS)

uint_8* heap_alloc(tsize size) {
    uint_8* result = NULL;
    while (result == NULL) {
        result = HeapAlloc(GetProcessHeap(), 0, size);
    }
    return result;
}
uint_8* heap_realloc(uint_8* address, tsize size) {
    uint_8* result = NULL;
    while (result == NULL) {
        result = HeapReAlloc(GetProcessHeap(), 0, address, size);
    }
    return result;
}
void heap_free(uint_8* address) {
    if (address != NULL) {
        HeapFree(GetProcessHeap(), 0, address);
    }
}
void heap_copy(uint_8* address, uint_8* source, tsize size) {
    CopyMemory(address, source, size);
}
void heap_move(uint_8* address, uint_8* source, tsize size) {
    MoveMemory(address, source, size);
}
void heap_set(uint_8* address, uint_8 source, tsize size) {
    FillMemory(address, size, source);
}

#endif
