# TODO List

## TODO

1. __Memory__
    1. __Low__: kernel
        1. __Type__ -> `0.1.0`
        2. __Heap__ -> `0.1.0`
2. __IO__
    1. __Low__: kernel
        1. __File__ -> `0.1.0` (full+kre => `0.2.0`)
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
    1. __High__
        1. __FSM__ -> `0.1.0`
        2. __KRE__ -> `0.1.0`
6. __Processor__
    1. __Low__: kernel
        1. __Thread__ -> `0.1.0` (affinity + priority => `0.2.0`)
        2. __Process__ -> `0.1.0` (affinity + priority => `0.2.0`)
    2. __High__
        1. __ThreadPool__ -> `0.1.0`
        2. __ProcessPool__ -> `0.1.0`
7. __Local__
    1. __Low__: kernel
        1. __Time__ -> `0.2.0`
        2. __Date__ -> `0.2.0`
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
12. __OK__ - implement File low (+file +poller)
13. __OK__ - refactor code with vtable
14. __OK__ refactor code with (convert return result; at methods do not need Builder Pattern (return x;)) + _new => _new_object
15. __OK__ - app_init -> call x_init all parts
16. implement algo high kre
17. implement algo high fsm + esm (extended) + hsm (herachical)
18. implement ipc low Win32
19. implement ipc low Android
20. implement processor high ThreadPool, ProcessPool
21. implement processor low Thread, Process Posix
22. implement processor low Thread, Process(MinGW fork) Win32
23. implement processor low Thread, Process Android
24. implement io low File Posix
25. implement io low File Win32
26. implement io low Poller Linux(epoll)
27. implement io low Poller Mac, BSD, IOS(kqueue)
28. implement io low Poller Win32(iocp)
29. implement io low Poller Android(poll)
30. launch (0.1.0) -> test
31. implement ipc high Monitor, Barrier, Latch
32. implement dsa high Set, Hash
33. implement processor low Thread, Process (affinity, priority, policy) Posix
34. implement processor low Thread, Process (affinity, priority, policy) Win32
35. implement processor low Thread, Process (affinity, priority, policy) Android
36. implement local low Time, Date Posix
37. implement local low Time, Date Win32
38. launch (0.2.0) -> test
39. launch (0.3.0) -> test => Network
40. launch (0.4.0) -> test => (Security)?

## Now

1. __OK__ - x_anonymous_new, _free below them
2. __OK__ - comment mutex and cond and svalue check