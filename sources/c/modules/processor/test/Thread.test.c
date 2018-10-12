#include <low/Thread.h>

#include <assert.h>

int function(void* arg) {
    int* a = arg;

    assert(*a == 4);

    return 5;
}

int main() {
    Thread* t1 = thread_new_object(0);

    t1->vtable->start(t1, function, 4);

    assert(t1->vtable->join(t1) == 5);

    t1->vtable->stop(t1);

    thread_free(t1);
}