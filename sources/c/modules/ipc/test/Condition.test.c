#include <low/Condition.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function(void* arg){
    Condition* condition = arg;

    assert(condition->vtable->wait(condition, 3000) == -1);

    assert(condition->vtable->wait(condition, UINT_64_MAX) == 0);

    return 0;
}

int main() {
    kstd_init();

    Condition* condition = condition_new_object(NULL);

    Thread* t = thread_new_object(0);
    t->vtable->start(t, function, condition);

    sleep(5);
    assert(condition->vtable->signal(condition, 1) == 0);

    t->vtable->join(t);

    thread_free(t);

    condition_free(condition);
}