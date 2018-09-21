# TODO List

## TODO

1. __Memory__
    1. __Low__: kernel
        1. __Type__ -> `0.1.0`
        2. __Heap__ -> `0.1.0`
2. __IO__
    1. __Low__: kernel
        1. __File__ -> `0.1.0`
        2. __Poller__ -> `0.1.0`
3. __IPC__
    1. __Low__: kernel
        1. __Mutex__ -> `0.1.0`
        2. __Condition__ -> `0.1.0`
        3. __Semaphore__ -> `0.1.0`
        4. __Message__ -> `0.1.0`
    2. __High__
        1. __Lock__ -> `0.1.0`
        2. __RWLock__ -> `0.1.0`
        3. __Monitor__ -> `0.2.0`
        4. __Barrier__ -> `0.2.0`
        5. __Latch__ -> `0.2.0`
4. __DSA__
    1. __Low__
        1. __String__ -> `0.1.0`
    2. __High__
        1. __ArrayList__ -> `0.1.0`
        2. __LinkedList__ -> `0.1.0`
        3. __Dequeue__ -> `0.1.0`
        4. __Set__ -> `0.2.0`
        5. __Hash__ -> `0.2.0`
5. __Algo__
    1. __Low__
        1. __Math__ -> `0.2.0`
    2. __High__
        1. __KRE__ -> `0.1.0`
6. __Processor__
    1. __Low__: kernel
        1. __Thread__ -> `0.1.0` (affinity + priority => `0.3.0`)
        2. __Process__ -> `0.1.0` (affinity + priority => `0.3.0`)
    2. __High__
        1. __ThreadPool__ -> `0.1.0`
        2. __ProcessPool__ -> `0.1.0`
7. __Local__
    1. __Low__: kernel
        1. __Time__ -> `0.3.0`
        2. __Date__ -> `0.3.0`
        3. __Locale__ -> `0.3.0`
8. __Net__
    1. __Low__: kernel
        1. __TCP__ -> `0.4.0`
        2. __UDP__ -> `0.4.0`
        3. __TLS__ -> `0.5.0`
        4. __DTLS__ -> `0.5.0`
    2. __High__
        1. __HTTP__ -> `0.6.0`
        2. __WS__ -> `0.6.0`
        3. __SSE__ -> `0.6.0`
9. __Security__
    1. __High__ -> `0.5.0`

## Check List

1. __OK__ - implement Local low
2. __OK__ - implement IPC low (+Atomic)
3. __OK__ - implement IPC high (+Message)
4. __OK__ - Change struct X self -> X self
5. __OK__ - atomic _new, _free IPC low
6. __OK__ - implement Thread (+affinity +priority)
7. __OK__ - implement Process (+affinity +priority)
8. __OK__ - implement ThreadPool
9. __OK__ - implement ProcessPool
10. __OK__ - implement DSA low
11. __OK__ - implement DSA high
12. implement Algo low (+math)
13. implement Algo high (+kre)
14. implement File low (+file +poller)
15. refactor code with vtable

## Now

1. __OK__ - x_anonymous_new, _free below them
2. __OK__ - comment mutex and cond and svalue check