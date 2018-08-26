# Application Programming Interface

## Library API

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