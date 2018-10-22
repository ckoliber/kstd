#include <kstd.h>
#include <stdlib.h>

// memory module
#include <low/Heap.h>
#include <low/Type.h>

// local module
#include <low/Date.h>

// dsa module
#include <high/ArrayList.h>
#include <high/Dequeue.h>
#include <high/LinkedList.h>
#include <high/Queue.h>
#include <high/Stack.h>
#include <low/String.h>

// processor module
#include <high/ProcessPool.h>
#include <high/ThreadPool.h>
#include <low/Process.h>
#include <low/Thread.h>

// ipc module
#include <low/ErrorCheckLock.h>
#include <low/Message.h>
#include <low/Monitor.h>
#include <low/MutexLock.h>
#include <low/ReadWriteLock.h>
#include <low/ReentrantLock.h>
#include <low/Semaphore.h>
#include <low/Share.h>

// local methods
void modules_init();

// implement methods
void modules_init() {
    // init memory module

    // init local module

    // init dsa module
    arraylist_init();
    dequeue_init();
    linkedlist_init();
    linkedlistiterator_init();
    queue_init();
    stack_init();
    string_init();

    // init processor module
    processpool_init();
    threadpool_init();
    process_init();
    thread_init();

    // init ipc module
    errorchecklock_init();
    message_init();
    monitor_init();
    mutexlock_init();
    readwritelock_init();
    reentrantlock_init();
    semaphore_init();
    share_init();
}

void kstd_init() {
    system("rm -Rf /dev/shm/*_share");

    modules_init();

    critical = NULL;
    critical = reentrantlock_new_object("critical");
}

void kstd_init_child(){
    critical = NULL;
    critical = reentrantlock_new_object("critical");
}
