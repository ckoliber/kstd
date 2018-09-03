#include <memory/low/Type.h>

typedef struct Message {
    SignedInt enqueue(struct Message* self, Void* item, UnsignedLong timeout);
    SignedInt dequeue(struct Message* self, Void** item, UnsignedLong timeout);
} Message;

Message* message_new(Char* name, UnsignedInt max, Size item);
Void message_free(Message* message);