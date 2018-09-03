#include <memory/low/Type.h>

Void* heap_alloc(Size size);
Void heap_copy(Void* address, Void* source, Size size);
Void heap_move(Void* address, Void* source, Size size);
Void heap_set(Void* address, SignedByte source, Size size);
Void* heap_realloc(Void* address, Size size);
Void heap_free(Void* address);