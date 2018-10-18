#include <low/Share.h>
#include <low/MutexLock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>
#include <unistd.h>

int function_1(uint_8* arg){
    Share* share = (Share *) arg;

    assert(share->vtable->address(share) != NULL);

    assert(share->vtable->connections(share) == 1);

    assert(*(int*) share->vtable->address(share) == 4);

    return 0;
}
int function_2(uint_8* arg){
    Share* share = share_new_object("test_share", sizeof(int), 0);

    assert(share->vtable->address(share) != NULL);

    assert(share->vtable->connections(share) == 2);

    assert(*(int*) share->vtable->address(share) == 4);

    share_free(share);

    return 0;
}

void share_test_anonymous();
void share_test_named();

void share_test_anonymous(){
    Share* share = share_new_object(NULL, sizeof(int), 0);

    *(int*) share->vtable->address(share) = 4;

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function_1, (uint_8*) share);

    t->vtable->join(t);

    thread_free(t);

    share_free(share);
}
void share_test_named(){
    Share* share = share_new_object("test_share", sizeof(int), 0);

    *(int*) share->vtable->address(share) = 4;

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function_2, NULL);

    t->vtable->join(t);

    thread_free(t);

    share_free(share);
}

int main() {
    kstd_init();

    share_test_anonymous();
    share_test_named();
}