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

____________________________________________________

### IO

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
    * __Comment__: `mode`-> `0`: normal, `1`-> recursive, `2`-> errorcheck
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
    * Done
2. __Condition__ -> `0.1.0`
    * __Comment__: `timeout`-> `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `count`-> `1`: signal, `number`: signal count, `UNSIGNED_INT_MAX`: broadcast
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
    * Done
3. __Semaphore__ -> `0.1.0`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
    * Done
4. __Message__ -> `0.1.0`
    * __Comment__: `max`-> Message queue max size
    * __Comment__: `item`-> Message queue item size
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
    * Done

#### High

1. __Lock__ -> `0.1.0`
    * __Comment__: `mode`-> `0`: normal, `1`-> reentrant(recursive), `2`-> errorcheck
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
    * Done
2. __RWLock__ -> `0.1.0`
    * __Comment__: `timeout`-> `0`: test, `number`: timed, `UNSIGNED_LONG_MAX`: infinity
    * __Comment__: `name`-> `NULL`: anonymous (between threads), `name`: named (between processes)
    * Done

____________________________________________________

### DSA

#### High

1. __ArrayList__ -> `0.1.0`
    * Done
2. __LinkedList__ -> `0.1.0`
    * Done
3. __Dequeue__ -> `0.1.0`
    * if `comperator` is not `NULL` queue is a `PriorityDequeue`
    * Done
4. __Set__ -> `0.2.0`
    * __concurrent, normal set (memory, speed optimization)__
5. __Hash__ -> `0.2.0`
    * __concurrent, normal hashmap (memory, speed optimization - sparsehash + densehash)__

____________________________________________________

### Processor

#### Low

1. __Thread__ -> `0.1.0`
    * __Comment__ `priority`+`affinity` -> `get=-1` -> get, `get=number` -> set
    * __Comment__ `stack`: `-1` -> default size, `number` -> custom stack size
    * Done
2. __Process__ -> `0.1.0`
    * __Comment__ `priority`+`affinity` -> `get=-1` -> get, `get=number` -> set
    * __Comment__ `stack`: `-1` -> default size, `number` -> custom stack size
    * Done

#### High

1. __ThreadPool__ -> `0.1.0`
    * Done
2. __ProcessPool__ -> `0.1.0`
    * Done

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