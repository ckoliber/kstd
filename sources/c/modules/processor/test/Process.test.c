#include <low/Process.h>

#include <assert.h>

int function(void* arg) {
    int* a = arg;

    assert(*a == 4);

    return 5;
}

int main() {
    Process* p1 = process_new_object();

    p1->vtable->start(p1, function, 4);

    assert(p1->vtable->join(p1) == 5);

    p1->vtable->stop(p1);

    process_free(p1);
}