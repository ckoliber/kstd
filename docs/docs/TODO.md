# TODO List

## TODO

1. __Low__: libc, kernel(linux, bsd, darwin, win32api)
    1. __ITC (Inter Thread Communication)__
        1. __Low__: OS, Hardware
            1. __Mutex__ -> `0.1.0`
            2. __Cond__ -> `0.1.0`
        2. __High__: ITC/Low
            1. __Lock__ -> `0.1.0`
            2. __RWLock__ -> `0.1.0`
            3. __Semaphore__ -> `0.1.0`
            4. __Monitor__ -> `0.2.0`
            5. __Barrier__ -> `0.2.0`
            6. __Latch__ -> `0.2.0`
    2. __IPC (Inter Process Communication)__ -> `0.5.0`
    3. __Processor__
        1. __Low__: OS
            1. __Thread__ -> `0.1.0`
            2. __Process__ -> `0.1.0`
        2. __High__: Processor/Low
            1. __ThreadPool__ -> `0.1.0`
            2. __ProcessPool__ -> `0.2.0`
    4. __Local__: OS
        1. __Time__ -> `0.5.0`
        2. __Date__ -> `0.5.0`
        3. __Locale__ -> `0.5.0`
    5. __DSA__: ITC/High
        1. __ArrayList__ -> `0.1.0`
        2. __LinkedList__ -> `0.1.0`
        3. __Dequeue__ -> `0.1.0`
        4. __Queue__ -> `0.1.0`
        5. __Stack__ -> `0.1.0`
        6. __Set__ -> `0.2.0`
        7. __Hash__ -> `0.2.0`
    6. __Security__ -> `0.3.0`
2. __IO__
    1. __Memory__ -> `0.1.0`
    2. __File__
        1. __File__ -> `0.1.0`
        2. __Poller__ -> `0.1.0`
    3. __Net__: DSA, Low
        1. __TCP__ -> `0.2.0`
        2. __UDP__ -> `0.2.0`
        3. __TLS__ -> `0.3.0`
        4. __HTTP__ -> `0.3.0`
        5. __HTTPS__ -> `0.3.0`
        6. __WS__ -> `0.4.0`
        7. __WSS__ -> `0.4.0`
        8. __SSE__ -> `0.4.0`