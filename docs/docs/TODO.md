# TODO List

## TODO

1. __Memory__
    1. __Low__: kernel
        1. __Type__ -> `0.1.0`
        2. __Heap__ -> `0.1.0`
        3. __Share__ -> `0.1.0`
2. __File__
    1. __Low__: kernel
        1. __File__ -> `0.1.0`
        2. __Poller__ -> `0.1.0`
3. __IPC__
    1. __Low__: kernel
        1. __Mutex__ -> `0.1.0`
        2. __Condition__ -> `0.1.0`
        3. __Semaphore__ -> `0.1.0`
        4. __Atomic__ -> `0.1.0`
    2. __High__
        1. __Lock__ -> `0.1.0`
        2. __RWLock__ -> `0.1.0`
        3. __Message__ -> `0.1.0`
        4. __Monitor__ -> `0.2.0`
        5. __Barrier__ -> `0.2.0`
        6. __Latch__ -> `0.2.0`
4. __DSA__
    1. __Low__
        1. __String__ -> `0.1.0`
    2. __High__
        1. __ArrayList__ -> `0.1.0`
        2. __LinkedList__ -> `0.1.0`
        3. __Dequeue__ -> `0.1.0`
        4. __Queue__ -> `0.1.0`
        5. __Stack__ -> `0.1.0`
        6. __Set__ -> `0.2.0`
        7. __Hash__ -> `0.2.0`
5. __Algo__
    1. __Low__
        1. __Math__ -> `0.2.0`
    2. __High__
        1. __KRE__ -> `0.1.0`
6. __Processor__
    1. __Low__: kernel
        1. __Thread__ -> `0.1.0`
        2. __Process__ -> `0.1.0`
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
2. implement IPC low (+Atomic)
3. implement IPC high (+Message)
4. atomic _new, _free IPC low
5. implement Thread (+affinity +priority)
6. implement Process (+affinity +priority)
7. implement ThreadPool
8. implement ProcessPool
9. implement DSA low
10. implement DSA high
11. implement Algo low (+math)
12. implement Algo high (+kre)
13. implement File low (+file +poller)