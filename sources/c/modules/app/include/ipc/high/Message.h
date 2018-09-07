#include <memory/low/Type.h>

typedef struct Message {
    int enqueue(struct Message* self, void* item, uint_64 timeout);
    int dequeue(struct Message* self, void** item, uint_64 timeout);
} Message;

Message* message_new(char* name, int max, tsize item);
void message_free(Message* message);