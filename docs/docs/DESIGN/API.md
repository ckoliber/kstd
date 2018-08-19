# Application Programming Interface

## Library API

### Low

#### Concurrency

##### Lock

* __Struct__:
    ```c
    struct Lock{
        int lock();
        int unlock();
        int trylock(int timeout);
    }
    ```
* __Function__: `Lock* lock_new()`
* __Function__: `Lock* lock_new(int id)`
* __Function__: `void lock_free(Lock* lock)`

##### Semaphore

* __Struct__:
    ```c
    struct Semaphore{
        int init(int count);
        int wait(int count);
        int post(int count);
        int get();
    }
    ```
* __Function__: `Semaphore* semaphore_new()`
* __Function__: `Semaphore* semaphore_new(int id)`
* __Function__: `void semaphore_free(Semaphore* semaphore)`

##### RWLock

* __Struct__:
    ```c
    struct RWLock{
        int rlock();
        int wlock();
        int runlock();
        int wunlock();
        int tryrlock(int timeout);
        int trywlock(int timeout);
    }
    ```
* __Function__: `RWLock* rwlock_new()`
* __Function__: `RWLock* rwlock_new(int id)`
* __Function__: `void rwlock_free(RWLock* rwlock)`

##### Barrier

* __Struct__:
    ```c
    struct Barrier{
        int reset();
        int await();
        int tryawait(int timeout);
    }
    ```
* __Function__: `Barrier* barrier_new(int size)`
* __Function__: `void barrier_free(Barrier* barrier)`

##### Latch

* __Struct__:
    ```c
    struct Latch{
        int count();
        int await();
        int tryawait(int timeout);
    }
    ```
* __Function__: `Barrier* barrier_new(int size)`
* __Function__: `void barrier_free(Barrier* barrier)`

#### Process

##### Pool

* __Struct__:
    ```c
    struct Pool{
        int start();
        int post(void (*function)(void* arg), void* arg);
        int stop();
    }
    ```
* __Function__: `Pool* pool_new(int size, int type)`
* __Function__: `void pool_free(Pool* pool)`

#### DSA

##### ArrayList

* __Struct__:
    ```c
    struct ArrayList{
        int add(void *data);
        void* put(int position, void *data);
        void* remove(int position);
        void* get(int position);
        int indexof(void *data);
        int size();
    }
    ```
* __Function__: `ArrayList* arraylist_new(int concurrent, float growth_factor, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void arraylist_free(ArrayList* arraylist)`

##### LinkedList

* __Struct__:
    ```c
    struct LinkedList{
        int add(void *data);
        void* put(int position, void *data);
        void* remove(int position);
        void* get(int position);
        int indexof(void *data);
        int size();
    }
    ```
* __Function__: `ArrayList* arraylist_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* __Function__: `void arraylist_free(ArrayList* arraylist)`

##### Queue

* __Struct__:
    ```c
    struct Queue{
        int enqueue(int front, void* data)
        void* dequeue(int front);
        void* blockdequeue(int front);
        void* tryblockdequeue(int front, int timeout);
        int size();
    }
    ```
* __Function__: `Queue* queue_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* if `comperator` is not `NULL` queue is a `PriorityQueue`
* __Function__: `void queue_free(Queue* queue)`

##### Stack

* __Struct__:
    ```c
    struct Stack{
        int push(void* data)
        void* pop();
        void* blockpop();
        void* tryblockpop(int timeout);
        int size();
    }
    ```
* __Function__: `Stack* stack_new(int concurrent, int (*comperator)(void* item1, void* itme2))`
* if `comperator` is not `NULL` stack is a `PriorityStack`
* __Function__: `void stack_free(Stack* stack)`

##### Set

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

#### Security

____________________________________________

### IO

#### Memory

* __Function__: `void* memory_alloc(int size)`
* __Function__: `void* memory_calloc(int count, int size)`
* __Function__: `void* memory_realloc(void* address, int size)`
* __Function__: `void memory_free(void* address)`

#### File

##### FD

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

##### Pipe

#### Net

##### TCP

##### UDP

##### TLS

##### HTTP

##### HTTPS

##### WS

##### WSS

##### SSE