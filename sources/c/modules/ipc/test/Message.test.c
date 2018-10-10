#include <low/Message.h>

#include <assert.h>

int main() {
    Message* message = message_new_object(NULL, 10, sizeof(int));

    message_free(message);
}