#include <low/Process.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>
#include <stdio.h>

int test_function(void* arg) {
    assert((int) arg == 4);

    return 5;
}

void test_process_start();
void test_process_join();
void test_process_id();
void test_process_stop();

void test_process_start(){
    Process* process = process_new_object();

    assert(process->vtable->start(process, test_function, (void *) 4) == 0);

    process->vtable->join(process);

    process_free(process);
}
void test_process_join(){
    Process* process = process_new_object();

    process->vtable->start(process, test_function, (void *) 4);

    assert(process->vtable->join(process) == 5);

    process_free(process);
}
void test_process_id(){
    Process* process = process_new_object();

    process->vtable->start(process, test_function, (void *) 4);

    assert(process->vtable->id(process) > 0);

    process->vtable->stop(process);

    process_free(process);
}
void test_process_stop(){
    Process* process = process_new_object();

    process->vtable->start(process, test_function, (void *) 4);

    process->vtable->stop(process);

    process_free(process);
}

int main() {
    kstd_init();

    test_process_start();
    test_process_join();
    test_process_id();
    test_process_stop();
}