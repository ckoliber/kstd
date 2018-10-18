# kstd (KoLiBer Standard Library)

__KSTD__ is cross platform library for creating __high performance__ `C` application that can run on multiple OS'es, __without__ any library __dependency__.
KSTD only depends on OS system calls.
It's fully modular application, it source code breaked into two level codes, __high level__ codes (depends on low level codes) and __low level__ codes (depends on OS syscalls).
KSTD was developed over __Object Oriented__ paradigm (OOC) and it is easily expandable.

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

## Linking

After building library, you can link `.so` or `.a` or `.dll` file to your application and enjoy __KSTD__ :)