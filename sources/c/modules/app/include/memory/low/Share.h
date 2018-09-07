#include <memory/low/Type.h>

typedef struct Share {
    Void* (*address)(struct Share* self);
    Void (*flush)(struct Share* self, Size size);
} Share;

Share* share_new(Char* name, Size offset, Size size);
Void share_free(Share* share);