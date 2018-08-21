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
        1. __Low (based on `OS`)__: OS
            1. __Thread__ -> `0.1.0`
            2. __Process__ -> `0.1.0`
        2. __High (based on `Low`)__: Processor/Low
            1. __ThreadPool__ -> `0.1.0`
            2. __ProcessPool__ -> `0.1.0`
    4. __Local__: OS
        1. __Time__ -> `0.5.0`
        2. __Date__ -> `0.5.0`
        3. __Locale__ -> `0.5.0`
    5. __DSA__: ITC/High
        1. __ArrayList__ -> `0.1.0`
        2. __LinkedList__ -> `0.1.0`
        3. __Queue__ -> `0.1.0`
        4. __Stack__ -> `0.1.0`
        5. __Set__ -> `0.2.0`
        6. __Hash__ -> `0.2.0`
    6. __Security__ -> `0.3.0`
2. __IO__
    1. __Memory__ -> `0.1.0`
        1. cross platform `malloc`, `calloc`, `realloc`, `free`
    2. __File__
        1. __FD__ -> `0.1.0`
            1. get fd from `stdio`, `memory`, `socket`, `file`
            2. control (fcntl) operators: `nonblock`, `block`
        2. __Poller__ -> `0.1.0`
            1. has fd set for polling array of fd's
            2. set's nonblocking flag on fd
        3. __Pipe__ -> `0.2.0`
            1. filter pipe
            2. map pipe
            3. reduce pipe
    3. __Net__: DSA, Low
        1. __TCP__ -> `0.2.0`
            1. TCP socket `server` and `client`
        2. __UDP__ -> `0.2.0`
            1. UDP socket `server` and `client`
        3. __TLS__ -> `0.3.0`
            1. TLS socket `server` and `client`
        4. __HTTP__ -> `0.3.0`
            1. HTTP `server` and `client`
        5. __HTTPS__ -> `0.3.0`
            1. HTTPS `server` and `client`
        6. __WS__ -> `0.4.0`
            1. WebSocket `server` and `client`
        7. __WSS__ -> `0.4.0`
            1. WebSocketSecure `server` and `client`
        8. __SSE__ -> `0.4.0`
            1. ServerSendEvent `server` and `client`