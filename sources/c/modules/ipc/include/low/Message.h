#include <low/Type.h>

// structs
typedef struct Message Message;
typedef struct Message_VTable Message_VTable;

// implement structs
// vtable + private data problem solve
struct Message {
    Message_VTable* vtable;
};

// vtable
struct Message_VTable {
    int (*enqueue)(Message* self, void* item, uint_64 timeout);
    int (*dequeue)(Message* self, void* item, uint_64 timeout);
    int (*size)(Message* self);
};

// init vtable
void message_init();

// new raw message
Message* message_new();

// free raw message
void message_free(Message* message);

// new message
Message* message_new_object(char* name, int max, tsize item);