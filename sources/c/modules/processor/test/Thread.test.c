#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>

int function(void* arg) {
    assert((int) arg == 4);

    return 5;
}

int main() {
    kstd_init();

    Thread* t1 = thread_new_object(0);

    t1->vtable->start(t1, function, (void *) 4);

    assert(t1->vtable->join(t1) == 5);

    t1->vtable->stop(t1);

    thread_free(t1);
}