#include <io/memory/Type.h>

Void* memory_alloc(Size size);
Void* memory_realloc(Void* address, Size size);
Void memory_free(Void* address);