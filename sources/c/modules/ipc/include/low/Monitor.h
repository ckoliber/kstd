#include <low/Type.h>

// structs
typedef struct Monitor Monitor;
typedef struct Monitor_VTable Monitor_VTable;

// implement structs
// vtable + private data problem solve
struct Monitor {
    Monitor_VTable* vtable;
};

// vtable
struct Monitor_VTable {
    int (*wait)(Monitor* self, uint_64 timeout);
    int (*notify)(Monitor* self);
    int (*notify_all)(Monitor* self);
};

// init vtable
void monitor_init();

// new raw monitor
Monitor* monitor_new();

// free raw monitor
void monitor_free(Monitor* monitor);

// new monitor
Monitor* monitor_new_anonymous();
Monitor* monitor_new_named(char* name);
