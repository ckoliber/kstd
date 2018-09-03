#ifdef __unix__

#include <io/memory/Memory.h>

#include <memory.h>
#include <stdlib.h>

Void* memory_alloc(Size size) {
    Void* result = NULL;
    while (result == NULL) {
        result = malloc(size);
    }
    return result;
}
Void* memory_realloc(Void* address, Size size) {
    Void* result = NULL;
    while (result == NULL) {
        result = realloc(address, size);
    }
    return result;
}
Void memory_free(Void* address) {
    if (address != NULL) {
        free(address);
    }
}

#endif