#include <memory/low/Type.h>

#define SHARE_ACCESS_READ 0b00000001
#define SHARE_ACCESS_WRITE 0b00000010
#define SHARE_ACCESS_EXEC 0b00000100

typedef struct Share {
    Void* (*address)(struct Share* self);
    Void (*flush)(struct Share* self);
} Share;

Share* share_new(Char* name, Size offset, Size size, SignedByte access);
Void share_free(Share* share);