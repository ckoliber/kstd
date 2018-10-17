#include <kstd.h>

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
#include <high/ReentrantLock.h>
#include <high/ReadWriteLock.h>
#include <low/Monitor.h>
#include <low/Message.h>
#include <low/MutexLock.h>
#include <low/Semaphore.h>

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
    lock_init();
    rwlock_init();
    condition_init();
    message_init();
    mutex_init();
    semaphore_init();
}

void kstd_init() {
    modules_init();

    critical = mutex_new_object(1, "critical");
}