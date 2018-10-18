#include <low/Heap.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <memory.h>
#include <stdlib.h>

uint_8* heap_alloc(tsize size) {
    uint_8* result = NULL;
    while (result == NULL) {
        result = malloc(size);
    }
    return result;
}
uint_8* heap_realloc(uint_8* address, tsize size) {
    uint_8* result = NULL;
    while (result == NULL) {
        result = realloc(address, size);
    }
    return result;
}
void heap_free(uint_8* address) {
    if (address != NULL) {
        free(address);
    }
}
void heap_copy(uint_8* address, uint_8* source, tsize size) {
    memcpy(address, source, size);
}
void heap_move(uint_8* address, uint_8* source, tsize size) {
    memmove(address, source, size);
}
void heap_set(uint_8* address, uint_8 source, tsize size) {
    memset(address, source, size);
}

#endif
