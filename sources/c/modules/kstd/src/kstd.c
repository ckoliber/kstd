#include <kstd.h>

#include <high/ArrayList.h>
#include <high/Dequeue.h>
#include <high/LinkedList.h>
#include <high/Queue.h>
#include <high/Stack.h>
#include <low/String.h>

#include <high/Lock.h>
#include <high/RWLock.h>
#include <low/Condition.h>
#include <low/Message.h>
#include <low/Mutex.h>
#include <low/Semaphore.h>

#include <low/Date.h>

#include <low/Heap.h>
#include <low/Type.h>

#include <high/ProcessPool.h>
#include <high/ThreadPool.h>
#include <low/Process.h>
#include <low/Thread.h>

// local methods
void modules_init();

// implement methods
void modules_init() {
    // init dsa module
    arraylist_init();
    dequeue_init();
    linkedlist_init();
    linkedlistiterator_init();
    queue_init();
    stack_init();
    string_init();

    // init ipc module
    lock_init();
    rwlock_init();
    condition_init();
    message_init();
    mutex_init();
    semaphore_init();

    // init local module

    // init memory module

    // init processor module
    processpool_init();
    threadpool_init();
    process_init();
    thread_init();
}

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_WINDOWS)

void kstd_init() {
    modules_init();

    critical = mutex_new_object(1, "critical");
}

#elif defined(APP_ANDROID)

void kstd_init() {
    modules_init();

    critical = mutex_new(1, NULL);
}

#endif