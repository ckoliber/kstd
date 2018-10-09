#include <low/Heap.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <memory.h>
#include <stdlib.h>

void* heap_alloc(tsize size) {
    void* result = NULL;
    while (result == NULL) {
        result = malloc(size);
    }
    return result;
}
void* heap_realloc(void* address, tsize size) {
    void* result = NULL;
    while (result == NULL) {
        result = realloc(address, size);
    }
    return result;
}
void heap_free(void* address) {
    if (address != NULL) {
        free(address);
    }
}
void heap_copy(void* address, void* source, tsize size) {
    memcpy(address, source, size);
}
void heap_move(void* address, void* source, tsize size) {
    memmove(address, source, size);
}
void heap_set(void* address, char source, tsize size) {
    memset(address, source, size);
}

#endif