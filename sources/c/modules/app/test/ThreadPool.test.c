#include <low/processor/high/ThreadPool.h>

#include <io/memory/Memory.h>
#include <stdio.h>
#include <zconf.h>

int a = 1;

void task(void* arg) {
    int* item = arg;
    printf("%d - %d\n", a, *item);
    fflush(stdout);
    a++;
}

int main(int argc, char* argv[]) {
    ThreadPool* pool = threadpool_new(1);

    pool->start(pool);

    for (int cursor = 0; cursor < 100; cursor++) {
        int* item = heap_alloc(sizeof(int));
        *item = cursor;
        printf("%d\n", cursor);
        fflush(stdout);
        pool->post(pool, task, item);
    }

    pool->stop(pool, 0);

    threadpool_free(pool);
}