# kstd (KoLiBer Standard Library)

__KSTD__ is cross platform library for creating __high performance__ `C` application that can run on multiple OS'es, __without__ any library __dependency__.
KSTD only depends on OS system calls.
It's fully modular application, it source code breaked into two level codes, __high level__ codes (depends on low level codes) and __low level__ codes (depends on OS syscalls).
KSTD was developed over __Object Oriented__ paradigm (OOC) and it is easily expandable.

## Modules

__KSTD__ is a modular library, and you can use these modules

1. __dsa__: Abstract Data Types implementation
    1. __high__:
        1. __ArrayList__
        2. __Dequeue__
        3. __LinkedList__
        4. __Queue__
        5. __Stack__
    2. __low__:
        1. __String__
2. __ipc__: Inter Process Communication implementation ( `anonymous` or `named` )
    1. __low__:
        1. __Barrier__
        2. __ErrorCheckLock__
        3. __Latch__
        4. __Message__
        5. __Monitor__
        6. __MutexLock__
        7. __ReadWriteLock__
        8. __ReentrantLock__
        9. __Semaphore__
        10. __Share__
3. __kstd__: KSTD modules initializer
4. __local__: Date and Time implementation
    1. __low__:
        1. __Date__
        2. __Time__
5. __memory__: Heap and Type system implementation
    1. __low__:
        1. __Heap__
        2. __Type__
6. __processor__: Thread and Process implementation
    1. __high__
        1. __ThreadPool__
        2. __ProcessPool__
    2. __low__
        1. __Thread__
        2. __Process__

## Building

You can compile KSTD in two modes:

1. __debug (testing)__
2. __release__

And also you can set link type:

1. __static__
2. __dynamic__

So let's see the building command:

```bash
git clone https://github.com/koliberr136a1/kstd.git

cd sources/c
mkdir build
cd build

cmake -DBUILD=debug ..
make -j4
```

## Testing

[![asciicast](https://asciinema.org/a/4eH4c6deCDChJbZ47pBfKYRwZ.png)](https://asciinema.org/a/4eH4c6deCDChJbZ47pBfKYRwZ)

## Linking

After building library, you can link `.so` or `.a` or `.dll` file to your application and enjoy __KSTD__ :)