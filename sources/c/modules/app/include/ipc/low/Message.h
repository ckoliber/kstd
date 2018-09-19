#include <memory/low/Type.h>

typedef struct Message {
    int (*enqueue)(struct Message* self, void* item, uint64_t timeout);
    int (*dequeueu)(struct Message* self, void* item, uint64_t timeout);
} Message;

Message* message_new(char* name, int max, tsize item);
void message_free(Message* message);