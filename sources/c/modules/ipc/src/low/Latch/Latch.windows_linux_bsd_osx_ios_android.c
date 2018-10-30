#include <low/Latch.h>

#if defined(APP_WINDOWS) || defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <low/Share.h>
#include <low/String.h>
#include <low/Heap.h>
#include <low/ReentrantLock.h>

struct Latch_ {
    // self public object
    Latch self;

    // constructor data

    // private data
};

// vtable
Latch_VTable* latch_vtable;

// link methods

// implement methods
// vtable operators

// object allocation and deallocation operators
void latch_init() {
    // init vtable
    latch_vtable = heap_alloc(sizeof(Latch_VTable));
}
Latch* latch_new() {
    struct Latch_* latch_ = heap_alloc(sizeof(struct Latch_));

    // set vtable
    latch_->self.vtable = latch_vtable;

    // set constructor data

    // set private data

    return (Latch*)latch_;
}
void latch_free(Latch* latch) {
    struct Latch_* latch_ = (struct Latch_*)latch;

    // free private data

    // free constructor data

    // free self
    heap_free(latch_);
}
Latch* latch_new_anonymous(int count){
    struct Latch_* latch_ = (struct Latch_*)latch_new();

    // set constructor data

    // set private data

    return (Latch*)latch_;
}
Latch* latch_new_named(char* name, int count){
    struct Latch_* latch_ = (struct Latch_*)latch_new();

    // set constructor data

    // set private data
    // try lock critical
    if (critical != NULL) {
        critical->vtable->lock(critical, UINT_64_MAX);
    }



    // try unlock critical
    if (critical != NULL) {
        critical->vtable->unlock(critical);
    }

    return (Latch*)latch_;
}

#endif
