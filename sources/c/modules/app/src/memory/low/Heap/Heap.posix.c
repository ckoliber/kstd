#include <memory/low/Heap.h>

#if !defined(APP_WINDOWS)

#include <memory.h>
#include <stdlib.h>

Void* heap_alloc(Size size) {
    Void* result = NULL;
    while (result == NULL) {
        result = malloc(size);
    }
    return result;
}
Void heap_copy(Void* address, Void* source, Size size) {
    memcpy(address, source, size);
}
Void heap_move(Void* address, Void* source, Size size) {
    memmove(address, source, size);
}
Void heap_set(Void* address, SignedByte source, Size size) {
    memset(address, source, size);
}
Void* heap_realloc(Void* address, Size size) {
    Void* result = NULL;
    while (result == NULL) {
        result = realloc(address, size);
    }
    return result;
}
Void heap_free(Void* address) {
    if (address != NULL) {
        free(address);
    }
}

#endif