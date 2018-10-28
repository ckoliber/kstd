#include <low/Monitor.h>
#include <low/Thread.h>
#include <kstd.h>

#include <assert.h>

#if defined(APP_WINDOWS)
#define sleep(x) Sleep(x*1000)
#else
#include <unistd.h>
#define sleep(x) sleep(x)
#endif

int function(void* arg){
    Monitor* condition = arg;

    assert(condition->vtable->wait(condition, 500) == -1);

    assert(condition->vtable->wait(condition, UINT_64_MAX) == 0);

    return 0;
}

void test_monitor_wait();
void test_monitor_notify();
void test_monitor_notify_all();

void test_monitor_wait(){
    Monitor* monitor = monitor_new_anonymous();

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, monitor);

    sleep(1);

    monitor->vtable->notify(monitor);

    t->vtable->join(t);

    thread_free(t);

    monitor_free(monitor);
}
void test_monitor_notify(){
    Monitor* monitor = monitor_new_anonymous();

    Thread* t = thread_new_object(0);

    t->vtable->start(t, function, monitor);

    sleep(1);

    assert(monitor->vtable->notify(monitor) == 0);

    t->vtable->join(t);

    thread_free(t);

    monitor_free(monitor);
}
void test_monitor_notify_all(){
    Monitor* monitor = monitor_new_anonymous();

    Thread* t1 = thread_new_object(0);

    Thread* t2 = thread_new_object(0);

    t1->vtable->start(t1, function, monitor);

    t2->vtable->start(t2, function, monitor);

    sleep(1);

    assert(monitor->vtable->notify_all(monitor) == 0);

    t1->vtable->join(t1);

    t2->vtable->join(t2);

    thread_free(t1);

    thread_free(t2);

    monitor_free(monitor);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 10 ; a++){
        test_monitor_wait();
        test_monitor_notify();
        test_monitor_notify_all();
    }

}