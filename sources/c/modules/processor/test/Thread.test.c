#include <low/Thread.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

int test_function(void* arg) {
    assert((int)arg == 4);

    return 5;
}

void test_thread_start();
void test_thread_join();
void test_thread_id();
void test_thread_stop();

void test_thread_start() {
    Thread* thread = thread_new_object(0);

    assert(thread->vtable->start(thread, test_function, (void*) 4) == 0);

    thread->vtable->join(thread);

    thread_free(thread);
}
void test_thread_join() {
    Thread* thread = thread_new_object(0);

    thread->vtable->start(thread, test_function, (void*) 4);

    assert(thread->vtable->join(thread) == 5);

    thread_free(thread);
}
void test_thread_id() {
    Thread* thread = thread_new_object(0);

    thread->vtable->start(thread, test_function, (void*) 4);

    assert(thread->vtable->id(thread) > 0);

    thread->vtable->stop(thread);

    thread_free(thread);
}
void test_thread_stop() {
    Thread* thread = thread_new_object(0);

    thread->vtable->start(thread, test_function, (void*) 4);

    thread->vtable->stop(thread);

    thread_free(thread);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_thread_start();
        test_thread_join();
        test_thread_id();
        test_thread_stop();
    }
}