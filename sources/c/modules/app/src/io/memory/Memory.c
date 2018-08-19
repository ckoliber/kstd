#include <io/memory/Memory.h>
#include <stdlib.h>

void* memory_alloc(int size) {
    void* result = NULL;
    while (result == NULL) {
        result = malloc(size);
    }
    return result;
}

void* memory_calloc(int count, int size) {
    void* result = NULL;
    while (result == NULL) {
        result = calloc(count, size);
    }
    return result;
}

void* memory_realloc(void* address, int size) {
    void* result = NULL;
    while (result == NULL) {
        result = realloc(address, size);
    }
    return result;
}

void memory_free(void* address) {
    if (address != NULL) {
        free(address);
    }
}