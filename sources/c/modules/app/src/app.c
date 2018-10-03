#include <app.h>

#include <algo/high/kre.h>

#include <dsa/high/ArrayList.h>
#include <dsa/high/Dequeue.h>
#include <dsa/high/LinkedList.h>
#include <dsa/low/String.h>

#include <io/low/File.h>
#include <io/low/Poller.h>

#include <ipc/high/Lock.h>
#include <ipc/high/RWLock.h>
#include <ipc/low/Condition.h>
#include <ipc/low/Message.h>
#include <ipc/low/Mutex.h>
#include <ipc/low/Semaphore.h>

#include <local/low/Time.h>

#include <memory/low/Heap.h>
#include <memory/low/Type.h>

#include <processor/high/ProcessPool.h>
#include <processor/high/ThreadPool.h>
#include <processor/low/Process.h>
#include <processor/low/Thread.h>

// local methods
void modules_init();

// implement methods
void modules_init() {
    // init algo module
    kre_init();

    // init dsa module
    arraylist_init();
    dequeue_init();
    linkedlist_init();
    string_init();

    // init io module
    file_init();
    poller_init();

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
    threadpool_init();
}

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_WINDOWS)

void app_init() {
    critical = mutex_new(1, "/critical");

    modules_init();
}

#elif defined(APP_ANDROID)

void app_init() {
    critical = mutex_new(1, NULL);

    modules_init();
}

#endif