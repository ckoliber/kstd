# Application Programming Interface

## Library API



### Memory

#### Low

1. __Type__ -> `0.1.0` (regex => `0.2.0`)
    * __cross platform :__
        * 
    * __Function__: `void* memory_alloc(int size)`
    * __Function__: `void* memory_realloc(void* address, int size)`
    * __Function__: `void memory_free(void* address)`
2. __Heap__ -> `0.1.0`
    * __cross platform `alloc`, `realloc`, `free`__
    * __Function__: `void* memory_alloc(int size)`
    * __Function__: `void* memory_realloc(void* address, int size)`
    * __Function__: `void memory_free(void* address)`
3. __Share__ -> `0.1.0`

____________________________________________________

### File

#### Low

1. __File__ -> `0.1.0` (regex => `0.2.0`)
2. __Poller__ -> `0.1.0`

____________________________________________________

### IPC

#### Low

1. __Mutex__ -> `0.1.0`
2. __Condition__ -> `0.1.0`
3. __Semaphore__ -> `0.1.0`

#### High

1. __Lock__ -> `0.1.0`
2. __RWLock__ -> `0.1.0`
3. __MessageQueue__ -> `0.1.0`
4. __Monitor__ -> `0.2.0`
5. __Barrier__ -> `0.2.0`
6. __Latch__ -> `0.2.0`

____________________________________________________

### DSA

#### High

1. __ArrayList__ -> `0.1.0`
2. __LinkedList__ -> `0.1.0`
3. __Dequeue__ -> `0.1.0`
4. __Queue__ -> `0.1.0`
5. __Stack__ -> `0.1.0`
6. __Set__ -> `0.2.0`
7. __Hash__ -> `0.2.0`

____________________________________________________

### Processor

#### Low

1. __Thread__ -> `0.1.0`
2. __Process__ -> `0.1.0`

#### High

1. __ThreadPool__ -> `0.1.0`
2. __ProcessPool__ -> `0.1.0`

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























### Memory

#### Low

##### Type

##### Heap

##### Share

________________________________

### File

#### Low

##### File

##### Poller

________________________________

### IPC

#### Low

##### Mutex

* __Struct__:
    ```c
    struct Mutex{
        SignedByte lock();
        SignedByte timelock(UnsignedLong timeout);
        SignedByte unlock();
    }
    ```
* __Function__: `Mutex* mutex_new(Bool share, String name)`
* __Function__: `Void mutex_free(Mutex* mutex, Bool destroy)`
* __Comment__: `share`-> `false`: inter process, `true`: intra process
* __Comment__: `name`-> `NULL`: intra process -> anonymous, `name`: intra process -> named
* __Comment__: `destroy`-> `false`: 

##### Cond

* __Struct__:
    ```c
    struct Cond{
        SignedByte wait(SignedByte (*condition)(Void*), Void* arg1, Void (*critical)(Void*), Void* arg2);
        SignedByte timewait(SignedByte (*condition)(Void*), Void* arg1, Void (*critical)(Void*), Void* arg2, UnsignedLong timeout);
        SignedByte signal(Void (*critical)(Void*), Void* arg);
        SignedByte broadcast(Void (*critical)(Void*), Void* arg);
    }
    ```
* __Function__: `Cond* cond_new(String name)`
* __Function__: `void cond_free(Cond* cond, Bool destroy)`
* __Comment__: `condition`-> loop on the wait inner lock
* __Comment__: `critical`-> run function after wait and signal inner lock

#### High

##### Lock

##### RWLock

##### Semaphore

##### Monitor

##### Barrier

##### Latch

________________________________

### DSA

#### High

##### ArrayList

##### LinkedList

##### Dequeue

##### Queue

##### Stack

##### Set

##### Hash

________________________________

### Processor

#### Low

##### Thread

##### Process

#### High

##### ThreadPool

##### ProcessPool

________________________________

### Local

#### Low

##### Time

##### Date

##### Locale

________________________________

### Net

#### Low

##### TCP

##### UDP

##### TLS

#### High

##### HTTP

##### WS

##### SSE

________________________________

### Security

#### High

________________________________



### Low

> Low level important libraries over OS and Hardware

#### ITC (Inter Thread Communication)

> Thread Level Communication and Synchronization

##### Low

###### Mutex (Mutual Exclusion)

* __Struct__:
    ```c
    struct Mutex{
        int lock();
        int timelock(long int timeout);
        int unlock();
    }
    ```
* __Function__: `Mutex* mutex_new()`
* __Function__: `void mutex_free(Mutex* mutex)`

###### Cond (Conditional Variable)

* __Struct__:
    ```c
    struct Cond{
        int wait(int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2);
        int timewait(int (*condition)(void*), void* arg1, void (*critical)(void*), void* arg2, long int timeout);
        int signal(void (*critical)(void*), void* arg);
        int broadcast(void (*critical)(void*), void* arg);
    }
    ```
* __Function__: `Cond* cond_new()`
* __Function__: `void cond_free(Cond* cond)`
* __Comment__: `condition`-> loop on the wait inner lock
* __Comment__: `critical`-> run function after wait and signal inner lock

##### High

###### Lock

* __Struct__:
    ```c
    struct Lock{
        int lock();
        int timelock(long int timeout);
        int unlock();
    }
    ```
* __Function__: `Lock* lock_new()`
* __Function__: `void lock_free(Lock* lock)`

###### RWLock

* __Struct__:
    ```c
    struct RWLock{
        int readlock();
        int writelock();
        int timereadlock(long int timeout);
        int timewritelock(long int timeout);
        int readunlock();
        int writeunlock();
    }
    ```
* __Function__: `RWLock* rwlock_new()`
* __Function__: `void rwlock_free(RWLock* rwlock)`

###### Semaphore

* __Struct__:
    ```c
    struct Semaphore{
        int wait(int count);
        int timewait(int count, long int timeout);
        int post(int count);
        int get();
    }
    ```
* __Function__: `Semaphore* semaphore_new(int value)`
* __Function__: `void semaphore_free(Semaphore* semaphore)`

###### Monitor

###### Barrier

###### Latch

_________________________________________

#### IPC (Inter Process Communication)

> Process Level Communication using Socket and Pipe and MessageQueue and ShareMemory

_________________________________________

#### Processor

> OS Thread and Process

##### Low

###### Thread

* __Struct__:
    ```c
    struct Thread{
        int start(void (*function)(void*), void* arg);
        int join();
        int id();
        int stop();
    }
    ```
* __Function__: `Thread* thread_new()`
* __Function__: `void thread_free(Thread* thread)`

###### Process

* __Struct__:
    ```c
    struct Process{
        int start(char* command);
        int join();
        int id();
        int stop();
    }
    ```
* __Function__: `Process* process_new()`
* __Function__: `void process_free(Process* process)`

##### High

###### ThreadPool

* __Struct__:
    ```c
    struct ThreadPool{
        int start();
        int post(void (*function)(void*), void* arg);
        int stop(int force);
    }
    ```
* __Function__: `ThreadPool* threadpool_new(int size)`
* __Function__: `void threadpool_free(ThreadPool* threadpool)`

###### ProcessPool

* __Struct__:
    ```c
    struct ProcessPool{
        int start();
        int post(char* command);
        int stop();
    }
    ```
* __Function__: `ProcessPool* processpool_new(int size)`
* __Function__: `void processpool_free(ProcessPool* processpool)`

_________________________________________

#### Local

> OS Time and Date and Locale

##### Time

##### Date

##### Locale

_________________________________________

#### DSA

> Important and High Performance Data Structures

##### ArrayList

* __dynamic array__
* __concurrent, normal__

* __Struct__:
    ```c
    struct ArrayList{
        int add(void *item);
        int addto(int position, void *item);
        void* put(int position, void *item);
        void* remove(int position);
        void* get(int position);
        int indexof(void *item);
        int size();
    }
    ```
* __Function__: `ArrayList* arraylist_new(int mode, float factor, int (*comperator)(void*, void*))`
* __Function__: `void arraylist_free(ArrayList* arraylist)`
* __Comment__: `mode`-> 0 = normal, 1 = concurrent

##### LinkedList

* __circular doubly linked list__
* __concurrent, normal__

* __Struct__:
    ```c
    struct LinkedList{
        int add(void *item);
        int addto(int position, void *item);
        void* put(int position, void *item);
        void* remove(int position);
        void* get(int position);
        int indexof(void *item);
        int size();
    }
    ```
* __Function__: `LinkedList* linkedlist_new(int mode, int (*comperator)(void*, void*))`
* __Function__: `void linkedlist_free(LinkedList* linkedlist)`
* __Comment__: `mode`-> 0 = normal, 1 = concurrent

##### Dequeue

* __blocking, concurrent, normal__

* __Struct__:
    ```c
    struct Dequeue{
        int enqueue(int front, void* item);
        void* dequeue(int front, long int timeout);
        void* get(int front);
        int size();
    }
    ```
* if `comperator` is not `NULL` queue is a `PriorityDequeue`
* __Function__: `Dequeue* dequeue_new(int mode, int (*comperator)(void*, void*))`
* __Function__: `void dequeue_free(Dequeue* dequeue)`
* __Comment__: `mode`-> 0 = normal, 1 = concurrent, 2 = blocking

##### Queue

* __blocking, concurrent, normal__

* __Struct__:
    ```c
    struct Queue{
        int enqueue(void* item);
        void* dequeue(long int timeout);
        void* get();
        int size();
    }
    ```
* if `comperator` is not `NULL` queue is a `PriorityQueue`
* __Function__: `Queue* queue_new(int mode, int (*comperator)(void*, void*))`
* __Function__: `void queue_free(Queue* queue)`
* __Comment__: `mode`-> 0 = normal, 1 = concurrent, 2 = blocking

##### Stack

* __blocking, concurrent, normal__

* __Struct__:
    ```c
    struct Stack{
        int push(void* item);
        void* pop();
        void* get();
        int size();
    }
    ```
* if `comperator` is not `NULL` stack is a `PriorityStack`
* __Function__: `Stack* stack_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void stack_free(Stack* stack)`
* __Comment__: `mode`-> 0 = normal, 1 = concurrent, 2 = blocking

##### Set

* __concurrent, normal set (memory, speed optimization)__

* __Struct__:
    ```c
    struct Set{
        void* add(void* value, int size);
        void* remove(void* value, int size);
        int has(void* value, int size);
        int size();
    }
    struct SetIterator{
        int hasNext();
        void* next();
    }
    ```
* __Function__: `Set* set_new(int concurrent, int optimization)`
* __Function__: `void set_free(Set* set)`
* __Function__: `SetIterator* setiterator_new(Set* set)`
* __Function__: `void setiterator_free(SetIterator* setiterator)`

##### Hash -> SparseHash(Memory), DenseHash(Speed)

* __concurrent, normal hashmap (memory, speed optimization)__

* __Struct__:
    ```c
    struct Hash{
        void* put(void* key, int size, void* value);
        void* remove(void* key, int size);
        void* get(void* key, int size);
        int size();
    }
    struct HashIterator{
        int hasNext();
        void* next();
    }
    ```
* __Function__: `Hash* hash_new(int concurrent, int optimization)`
* __Function__: `void hash_free(Hash* hash)`
* __Function__: `HashIterator* hashiterator_new(Hash* hash)`
* __Function__: `void hashiterator_free(HashIterator* hashiterator)`

_________________________________________

#### Security

> Asymmetric and Symmetric encryption and decryption over OpenSSL

_________________________________________

### IO

> File, Memory, Socket, StandardIO operations

#### Memory

* __cross platform `malloc`, `calloc`, `realloc`, `free`__

* __Function__: `void* memory_alloc(int size)`
* __Function__: `void* memory_calloc(int count, int size)`
* __Function__: `void* memory_realloc(void* address, int size)`
* __Function__: `void memory_free(void* address)`

#### File

> Standard File Read and Write and Poll

##### File

* __get fd from `stdio`, `memory`, `socket`, `file`, `dir`, `pipe`__
* __control (fcntl) operators: `nonblock`, `block`__

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

#### Net

##### TCP

##### UDP

##### TLS

##### HTTP

##### HTTPS

##### WS

##### WSS

##### SSE

_________________________________________