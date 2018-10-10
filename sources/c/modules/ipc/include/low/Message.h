#include <low/Type.h>

// vtable
typedef struct Message_VTable {
    int (*enqueue)(struct Message* self, void* item, uint_64 timeout);
    int (*dequeueu)(struct Message* self, void* item, uint_64 timeout);
} Message_VTable;

// vtable + private data problem solve
typedef struct Message {
    Message_VTable* vtable;
} Message;

// init vtable
void message_init();

// new raw message
Message* message_new();

// free raw message
void message_free(Message* message);

// new message
Message* message_new_object(char* name, int max, tsize item);