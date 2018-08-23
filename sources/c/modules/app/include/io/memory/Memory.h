#if !defined(NULL)
#define NULL ((void*)0)
#endif

void* memory_alloc(int size);
void* memory_calloc(int count, int size);
void* memory_realloc(void* address, int size);
void memory_free(void* address);