#include <low/Message.h>
#include <low/Thread.h>
#include <kstd.h>


#include <assert.h>

int function(void* arg){
    Message* message = message_new_object("test_message", 10, sizeof(int));

    for(int a = 0 ; a < 1000 ; a++){
        int x;
        assert(message->vtable->dequeue(message, &x, 1000) == 0);
        assert(x == a);
    }

    message_free(message);
}

void test_message_enqueue();
void test_message_dequeue();
void test_message_size();

void test_message_enqueue(){
    Message* message = message_new_object(NULL, 1000, sizeof(int));

    for(int a = 0 ; a < 500 ; a++){
        assert(message->vtable->enqueue(message, &a, 0) == 0);
    }

    for(int a = 0 ; a < 500 ; a++){
        int x;
        assert(message->vtable->dequeue(message, &x, 0) == 0);
        assert(x == a);
    }

    message_free(message);
}
void test_message_dequeue(){
    Message* message = message_new_object("test_message", 10, sizeof(int));

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, NULL);

    for(int a = 0 ; a < 1000 ; a++){
        assert(message->vtable->enqueue(message, &a, 1000) == 0);
    }

    t->vtable->join(t);

    thread_free(t);

    message_free(message);
}
void test_message_size(){
    Message* message = message_new_object(NULL, 10, sizeof(int));

    message_free(message);
}

int main() {
    kstd_init();

    test_message_enqueue();
    test_message_dequeue();
    test_message_size();
}