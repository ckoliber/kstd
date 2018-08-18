# TODO List

## TODO

1. __Low__: libc, kernel(linux, bsd, darwin, win32api)
    1. __Concurrency__
        1. __Lock__ -> `0.1.0`
            1. basic lock implementation (`lock`, `unlock`)
        2. __Semaphore__ -> `0.1.0`
            1. based on `lock`
        3. __RWLock__ -> `0.1.0`
            1. based on `lock`
    2. __Process__
        1. __Pool__ -> `0.1.0`
            1. pool of `fiber` and `thread` and `process`
    3. __DSA__: Low
        1. __ArrayList__ -> `0.1.0`
            1. dynamic array
            2. dynamic growth factor
            3. concurrent, normal
        2. __LinkedList__ -> `0.1.0`
            1. circular doubly linked list
            2. concurrent, normal
        3. __Queue__ -> `0.1.0`
            1. blocking, concurrent, normal
        4. __Stack__ -> `0.1.0`
            1. blocking, concurrent, normal
        5. __Map__ -> `0.1.0`
            1. double array map
            2. concurrent, normal
        6. __Hash__ -> `0.1.0`
            1. simple mathematical hash function generator
        7. __Set__ -> `0.1.0`
            1. concurrent, normal
        8. __HashMap__ -> `0.1.0`
            1. concurrent, normal
    4. __Security__ -> `0.2.0`
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
        3. __Pipe__ -> `0.1.0`
            1. filter pipe
            2. map pipe
            3. reduce pipe
    3. __Net__: DSA, Low
        1. __TCP__ -> `0.1.0`
            1. TCP socket `server` and `client`
        2. __UDP__ -> `0.1.0`
            1. UDP socket `server` and `client`
        3. __TLS__ -> `0.1.0`
            1. TLS socket `server` and `client`
        4. __HTTP__ -> `0.1.0`
            1. HTTP `server` and `client`
        5. __HTTPS__ -> `0.1.0`
            1. HTTPS `server` and `client`
        6. __WS__ -> `0.1.0`
            1. WebSocket `server` and `client`
        7. __WSS__ -> `0.1.0`
            1. WebSocketSecure `server` and `client`
        8. __SSE__ -> `0.1.0`
            1. ServerSendEvent `server` and `client`