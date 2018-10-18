#include <low/Type.h>

uint_8* heap_alloc(tsize size);
uint_8* heap_realloc(uint_8* address, tsize size);
void heap_free(uint_8* address);
void heap_copy(uint_8* address, uint_8* source, tsize size);
void heap_move(uint_8* address, uint_8* source, tsize size);
void heap_set(uint_8* address, uint_8 source, tsize size);
