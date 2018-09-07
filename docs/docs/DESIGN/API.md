# Application Programming Interface

## Library API

### Memory

#### Low

1. __Type__ -> `0.1.0` (regex => `0.2.0`)
    * __cross platform `Data Types`:__
        * __[u]int_8__
        * __[u]int_16__
        * __[u]int_32__
        * __[u]int_64__
        * __void__
        * __bool__
        * __char__
        * __tsize__
        * __float__
        * __double__
    * Done
2. __Heap__ -> `0.1.0`
    * Done
3. __Share__ -> `0.1.0`
    * Done

____________________________________________________

### File

#### Low

1. __File__ -> `0.1.0` (regex => `0.2.0`)
    * __get fd from `console`, `socket`, `file`, `dir`, `pipe`, `dev`__
    * __control (fcntl) operators: `async`, `nonblock`, `block`__
    * __Struct__:
        ```c
        struct File{
            int read(void** data, int size);
            int write(void* data, int size);
            int flush();
            int wait();
            int cancel();
        }
        ```
    * __Function__: `File* file_new(char* uri)`
    * __Function__: `void file_free(File* file)`
    * __Comment__: `uri` ->
        1. `file://{path}`
        2. `pipe://{name}`
        3. `socket://{host}:{port}`
        4. `socket://{backlog}:{host}:{port}`
        5. `std://input`
        6. `std://output`
        7. `std://error`
2. __Poller__ -> `0.1.0`

____________________________________________________

### IPC

#### Low

1. __Mutex__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Mutex{
            SignedInt acquire(UnsignedLong timeout);
            SignedInt release();
        }
        ```
    * __Function__: `Mutex* mutex_new(Char* name)`
    * __Function__: `Void mutex_free(Mutex* mutex)`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
2. __Condition__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Condition{
            SignedInt wait(UnsignedLong timeout);
            SignedInt signal(UnsignedInt count);
        }
        ```
    * __Function__: `Condition* condition_new(Char* name)`
    * __Function__: `Void condition_free(Condition* condition)`
    * __Comment__: `timeout`-> `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `count`-> `1`: signal, `number`: signal count, `UNSIGNED_INT_MAX`: broadcast
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
3. __Semaphore__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Semaphore{
            SignedInt wait(UnsignedLong timeout);
            SignedInt post(UnsignedInt count);
            SignedInt get();
        }
        ```
    * __Function__: `Semaphore* semaphore_new(Char* name)`
    * __Function__: `Void semaphore_free(Semaphore* semaphore)`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)

#### High

1. __Lock__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Lock{
            SignedInt lock(UnsignedLong timeout);
            SignedInt unlock();
        }
        ```
    * __Function__: `Lock* lock_new(Char* name)`
    * __Function__: `Void lock_free(Lock* lock)`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
2. __RWLock__ -> `0.1.0`
    * __Struct__:
        ```c
        struct RWLock{
            SignedInt lock_read(UnsignedLong timeout);
            SignedInt lock_write(UnsignedLong timeout);
            SignedInt unlock_read();
            SignedInt unlock_write();
        }
        ```
    * __Function__: `RWLock* rwlock_new(Char* name)`
    * __Function__: `Void rwlock_free(RWLock* rwlock)`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
3. __Message__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Message{
            SignedInt enqueue(Void* item, UnsignedLong timeout);
            SignedInt dequeue(Void** item, UnsignedLong timeout);
        }
        ```
    * __Function__: `Message* message_new(Char* name, UnsignedInt max, Size item)`
    * __Function__: `Void message_free(Message* message)`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
4. __Monitor__ -> `0.2.0`
5. __Barrier__ -> `0.2.0`
6. __Latch__ -> `0.2.0`

____________________________________________________

### DSA

#### High

1. __ArrayList__ -> `0.1.0`
    * __Struct__:
        ```c
        struct ArrayList{
            SignedInt add(Void* item);
            SignedInt addto(SignedInt position, Void* item);
            Void* put(SignedInt position, Void* item);
            Void* remove(SignedInt position);
            Void* get(SignedInt position);
            SignedInt indexof(Void* item);
            SignedInt size();
        }
        enum ArrayListMode{
            ARRAYLIST_MODE_NORMAL,
            ARRAYLIST_MODE_CONCURRENT
        }
        ```
    * __Function__: `ArrayList* arraylist_new(ArrayListMode mode, Float factor, SignedInt (*comperator)(Void*, Void*))`
    * __Function__: `Void arraylist_free(ArrayList* arraylist)`
2. __LinkedList__ -> `0.1.0`
    * __Struct__:
        ```c
        struct LinkedList{
            SignedInt add(Void* item);
            SignedInt addto(SignedInt position, Void* item);
            Void* put(SignedInt position, Void* item);
            Void* remove(SignedInt position);
            Void* get(SignedInt position);
            SignedInt indexof(Void* item);
            SignedInt size();
        }
        enum LinkedListMode{
            LINKEDLIST_MODE_NORMAL,
            LINKEDLIST_MODE_CONCURRENT
        }
        ```
    * __Function__: `LinkedList* linkedlist_new(LinkedListMode mode, SignedInt (*comperator)(Void*, Void*))`
    * __Function__: `Void linkedlist_free(LinkedList* linkedlist)`
3. __Dequeue__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Dequeue{
            SignedInt enqueue(Bool front, Void* item, UnsignedLong timeout);
            Void* dequeue(Bool front, UnsignedLong timeout);
            Void* get(Bool front);
            SignedInt size();
        }
        enum DequeueMode{
            DEQUEUE_MODE_NORMAL,
            DEQUEUE_MODE_CONCURRENT,
            DEQUEUE_MODE_BLOCKING
        }
        ```
    * if `comperator` is not `NULL` queue is a `PriorityDequeue`
    * __Function__: `Dequeue* dequeue_new(DequeueMode mode, SignedInt max, SignedInt (*comperator)(Void*, Void*))`
    * __Function__: `Void dequeue_free(Dequeue* dequeue)`
4. __Queue__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Queue{
            SignedInt enqueue(Void* item, UnsignedLong timeout);
            Void* dequeue(UnsignedLong timeout);
            Void* get();
            SignedInt size();
        }
        enum QueueMode{
            QUEUE_MODE_NORMAL,
            QUEUE_MODE_CONCURRENT,
            QUEUE_MODE_BLOCKING
        }
        ```
    * if `comperator` is not `NULL` queue is a `PriorityQueue`
    * __Function__: `Queue* queue_new(QueueMode mode, SignedInt max, SignedInt (*comperator)(Void*, Void*))`
    * __Function__: `Void queue_free(Queue* queue)`
5. __Stack__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Stack{
            SignedInt push(Void* item, UnsignedLong timeout);
            Void* pop(UnsignedLong timeout);
            Void* get();
            SignedInt size();
        }
        enum StackMode{
            STACK_MODE_NORMAL,
            STACK_MODE_CONCURRENT,
            STACK_MODE_BLOCKING
        }
        ```
    * if `comperator` is not `NULL` stack is a `PriorityStack`
    * __Function__: `Stack* stack_new(StackMode mode, SignedInt max, SignedInt (*comperator)(Void*, Void*))`
    * __Function__: `void stack_free(Stack* stack)`
6. __Set__ -> `0.2.0`
    * __concurrent, normal set (memory, speed optimization)__
7. __Hash__ -> `0.2.0`
    * __concurrent, normal hashmap (memory, speed optimization - sparsehash + densehash)__

____________________________________________________

### Processor

#### Low

1. __Thread__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Thread{
            SignedInt priority(SignedInt priority);
            SignedInt affinity(SignedInt affinity);
            SignedInt start(Void (*function)(Void*), Void* arg);
            SignedInt join();
            SignedInt id();
            SignedInt stop();
        }
        ```
    * __Function__: `Thread* thread_new(Size stack)`
    * __Function__: `Void thread_free(Thread* thread)`
    * __Comment__ `priority`+`affinity` -> `get=-1` -> get, `get=number` -> set
    * __Comment__ `stack`: `-1` -> default size, `number` -> custom stack size
2. __Process__ -> `0.1.0`
    * __Struct__:
        ```c
        struct Process{
            SignedInt priority(SignedInt priority);
            SignedInt affinity(SignedInt affinity);
            SignedInt start(Char* command);
            SignedInt join();
            SignedInt id();
            SignedInt stop();
        }
        ```
    * __Function__: `Process* process_new()`
    * __Function__: `Void process_free(Process* process)`

#### High

1. __ThreadPool__ -> `0.1.0`
    * __Struct__:
        ```c
        struct ThreadPool{
            SignedInt start();
            SignedInt post(Void (*function)(Void*), Void* arg);
            SignedInt stop(Bool force);
        }
        ```
    * __Function__: `ThreadPool* threadpool_new(SignedInt size)`
    * __Function__: `Void threadpool_free(ThreadPool* threadpool)`
1. __ProcessPool__ -> `0.1.0`
    * __Struct__:
        ```c
        struct ProcessPool{
            SignedInt start();
            SignedInt post(Char* command);
            SignedInt stop();
        }
        ```
    * __Function__: `ProcessPool* processpool_new(SignedInt size)`
    * __Function__: `Void processpool_free(ProcessPool* processpool)`

____________________________________________________

### Local

#### Low

1. __Time__ -> `0.3.0`
2. __Date__ -> `0.3.0`
3. __Locale__ -> `0.3.0`

____________________________________________________

### Net

#### Low

1. __TCP__ -> `0.4.0`
2. __UDP__ -> `0.4.0`
3. __TLS__ -> `0.5.0`

#### High

1. __HTTP__ -> `0.6.0`
2. __WS__ -> `0.6.0`
3. __SSE__ -> `0.6.0`

____________________________________________________

### Security

1. __High__ -> `0.5.0`

____________________________________________________





#### File

> Standard File Read and Write and Poll

##### File


##### Poller

* __has fd set for polling array of fd's__
* __set's nonblocking flag on fd__

* __Struct__:
    ```c
    struct Poller{
        int add(FD* fd);
        int remove(FD* fd);
        int start();
        int stop();
    }
    ```
* __Function__: `Poller* poller_new(void (*onAccept)(FD* fd), void (*onClose)(FD* fd), void (*onRead)(FD* fd, void* data, int size))`
* __Function__: `void poller_free(Poller* poller)`