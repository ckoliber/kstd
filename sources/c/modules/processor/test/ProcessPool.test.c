#include <high/ProcessPool.h>

#include <assert.h>

void function(void* a) {
    int* num = a;
    assert(*num == 5);
}

int main() {
    ProcessPool* pool = processpool_new_object(2, sizeof(int));

    pool->vtable->start(pool);

    for (int a = 0; a < 100000; a++) {
        int x = 5;
        pool->vtable->post(pool, function, &x);
    }

    pool->vtable->stop(pool);

    processpool_free(pool);
}