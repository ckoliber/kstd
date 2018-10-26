#include <low/Message.h>
#include <low/Thread.h>
#include <low/Process.h>
#include <kstd.h>

#include <assert.h>

int function_1(uint_8* arg){
    Message* message = (Message *) arg;

    for(int a = 0 ; a < 100000 ; a++){
        int x;
        assert(message->vtable->dequeue(message, (uint_8*) &x, 1000) == 0);
        assert(x == a);
    }

    return 0;
}
int function_2(uint_8* arg){
    Message* message = message_new_object("test_message", 1024, sizeof(int));

    for(int a = 0 ; a < 100000 ; a++){
        int x;
        assert(message->vtable->dequeue(message, (uint_8*) &x, 1000) == 0);
        assert(x == a);
    }

    message_free(message);

    return 0;
}

void test_message_enqueue();
void test_message_dequeue_thread();
void test_message_dequeue_process();
void test_message_size();

void test_message_enqueue(){
    Message* message = message_new_object(NULL, 1000, sizeof(int));

    for(int a = 0 ; a < 500 ; a++){
        assert(message->vtable->enqueue(message, (uint_8*) &a, 0) == 0);
    }

    for(int a = 0 ; a < 500 ; a++){
        int x;
        assert(message->vtable->dequeue(message, (uint_8*) &x, 0) == 0);
        assert(x == a);
    }

    message_free(message);
}
void test_message_dequeue_thread(){
    Message* message = message_new_object(NULL, 10, sizeof(int));

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function_1, (uint_8 *) message);

    for(int a = 0 ; a < 100000 ; a++){
        assert(message->vtable->enqueue(message, (uint_8*) &a, 1000) == 0);
    }

    t->vtable->join(t);

    thread_free(t);

    message_free(message);
}
void test_message_dequeue_process(){
    Message* message = message_new_object("test_message", 1024, sizeof(int));

    Process* p = process_new_object();

    p->vtable->start(p, function_2, NULL);

    for(int a = 0 ; a < 100000 ; a++){
        assert(message->vtable->enqueue(message, (uint_8*) &a, 1000) == 0);
    }

    p->vtable->join(p);

    process_free(p);

    message_free(message);
}
void test_message_size(){
    Message* message = message_new_object(NULL, 10, sizeof(int));

    message_free(message);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_message_enqueue();
        test_message_dequeue_thread();
        test_message_dequeue_process();
        test_message_size();
    }
}