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
        int timelock(int timeout);
        int unlock();
    }
    ```
* __Function__: `Mutex* mutex_new()`
* __Function__: `void mutex_free(Mutex* mutex)`

###### Cond (Conditional Variable)

* __Struct__:
    ```c
    struct Cond{
        int wait();
        int timewait(int timeout);
        int signal();
        int broadcast();
    }
    ```
* __Function__: `Cond* cond_new()`
* __Function__: `void cond_free(Cond* cond)`

##### High

###### Lock

* __Struct__:
    ```c
    struct Lock{
        int lock();
        int timelock(int timeout);
        int unlock();
    }
    ```
* __Function__: `Lock* lock_new()`
* __Function__: `void lock_free(Lock* lock)`

###### RWLock

* __Struct__:
    ```c
    struct Lock{
        int readlock();
        int writelock();
        int timereadlock(int timeout);
        int timewritelock(int timeout);
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
        int wait();
        int timewait(int timeout);
        int post();
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
        int start(void (*function)(void* arg), void* arg);
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
        int post(void (*function)(void* arg), void* arg);
        int stop();
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
        void* put(int position, void *item);
        void* remove(int position);
        void* get(int position);
        int indexof(void *item);
        int size();
    }
    ```
* __Function__: `ArrayList* arraylist_new(int concurrent, float factor, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void arraylist_free(ArrayList* arraylist)`

##### LinkedList

* __circular doubly linked list__
* __concurrent, normal__

* __Struct__:
    ```c
    struct LinkedList{
        int add(void *item);
        void* put(int position, void *item);
        void* remove(int position);
        void* get(int position);
        int indexof(void *item);
        int size();
    }
    ```
* __Function__: `LinkedList* linkedlist_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void linkedlist_free(LinkedList* linkedlist)`

##### Queue

* __blocking, concurrent, normal__

* __Struct__:
    ```c
    struct Queue{
        int enqueue(int front, void* item);
        void* dequeue(int front);
        void* blockdequeue(int front);
        void* timeblockdequeue(int front, int timeout);
        int size();
    }
    ```
* if `comperator` is not `NULL` queue is a `PriorityQueue`
* __Function__: `Queue* queue_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void queue_free(Queue* queue)`

##### Stack

* __blocking, concurrent, normal__

* __Struct__:
    ```c
    struct Stack{
        int push(void* item);
        void* pop();
        void* blockpop();
        void* timeblockpop(int timeout);
        int size();
    }
    ```
* if `comperator` is not `NULL` stack is a `PriorityStack`
* __Function__: `Stack* stack_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void stack_free(Stack* stack)`

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

##### FD

* __get fd from `stdio`, `memory`, `socket`, `file`__
* __control (fcntl) operators: `nonblock`, `block`__

* __Struct__:
    ```c
    struct FD{
        int openfile(char* path);
        int opensocket(char* host, int port);
        FD* accept();
        int read(void* data, int size);
        int write(void* data, int size);
        int control(int nonblock);
        int close();
        int file(char* path);
        int socket(char* host, int* port);
    }
    ```
* __Function__: `FD* fd_new()`
* __Function__: `void fd_free(FD* fd)`

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