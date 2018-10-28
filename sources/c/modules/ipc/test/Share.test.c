#include <low/Share.h>
#include <low/MutexLock.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>

#if defined(APP_WINDOWS)
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#define sleep(x) sleep(x)
#endif

int function_1(void* arg){
    Share* share = arg;

    assert(share->vtable->address(share) != NULL);

    assert(share->vtable->connections(share) == 1);

    assert(*(int*) share->vtable->address(share) == 4);

    return 0;
}
int function_2(void* arg){
    Share* share = share_new_named("test_share", sizeof(int), 0);

    assert(share->vtable->address(share) != NULL);

    assert(share->vtable->connections(share) == 2);

    assert(*(int*) share->vtable->address(share) == 4);

    share_free(share);

    return 0;
}

void share_test_anonymous();
void share_test_named();

void share_test_anonymous(){
    Share* share = share_new_anonymous(sizeof(int), 0);

    *(int*) share->vtable->address(share) = 4;

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function_1, share);

    t->vtable->join(t);

    thread_free(t);

    share_free(share);
}
void share_test_named(){
    Share* share = share_new_named("test_share", sizeof(int), 0);

    *(int*) share->vtable->address(share) = 4;

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function_2, NULL);

    t->vtable->join(t);

    thread_free(t);

    share_free(share);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        share_test_anonymous();
        share_test_named();
    }
}