#include <low/Process.h>
#include <kstd.h>

#include <assert.h>

int function(void* arg) {
    assert((int) arg == 4);

    return 5;
}

int main() {
    kstd_init();

    Process* p1 = process_new_object();

    p1->vtable->start(p1, function, (void *) 4);

    int res = p1->vtable->join(p1);

    assert(res == 5);

    p1->vtable->stop(p1);

    process_free(p1);
}