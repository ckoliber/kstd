#include <memory/low/Type.h>

typedef struct Message {
    int (*enqueue)(struct Message* self, void* item, uint64_t timeout);
    int (*dequeueu)(struct Message* self, void* item, uint64_t timeout);
} Message;

// init vtable
void message_init();

// new raw message
Message* message_new();

// free raw message
void message_free(Message* message);

// new message
Message* message_new_object(char* name, int max, tsize item);