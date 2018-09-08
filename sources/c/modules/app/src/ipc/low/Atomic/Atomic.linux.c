#include <ipc/low/Condition.h>

#if defined(APP_LINUX)

#include <pthread.h>

extern pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void atomic_begin() {
}
void atomic_end() {
}

#endif