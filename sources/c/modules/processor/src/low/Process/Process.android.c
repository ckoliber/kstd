// android does not support posix processes and fork

#include <low/Process.h>

#if defined(APP_ANDROID)

// vtable

// link methods

// implement methods
// vtable operators

// object allocation and deallocation operators
void process_init() {
    // init vtable
}
Process* process_new() {
    return NULL;
}
void process_free(Process* process) {

}
Process* process_new_object() {
    return NULL;
}

// local process methods
int process_get_self() {
    return -1;
}
int process_get_parent() {
    return -1;
}

#endif