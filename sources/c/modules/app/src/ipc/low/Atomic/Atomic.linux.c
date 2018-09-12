#include <ipc/low/Atomic.h>

#if defined(APP_LINUX)

#include <fcntl.h>
#include <pthread.h>
#include <sys/mman.h>

char* atomic_name = "/atomic";

void atomic_init() {
    // create share memory
    int fd = shm_open(atomic_name, O_CREAT | O_RDWR, 0660);

    // init pthread mutex
    pthread_mutex_t* mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    pthread_mutexattr_t mattr;
    pthread_mutexattr_init(&mattr);
    pthread_mutexattr_setpshared(&mattr, 1);
    pthread_mutexattr_settype(&mattr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mutex, &mattr);
    pthread_mutexattr_destroy(&mattr);
    munmap(mutex, sizeof(pthread_mutex_t));

    // close share memory
    close(fd);
}
void atomic_begin() {
    // open share memory
    int fd = shm_open(atomic_name, O_RDWR, 0660);

    // acquire mutex
    pthread_mutex_t* mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    pthread_mutex_lock(mutex);
    munmap(mutex, sizeof(pthread_mutex_t));

    // close share memory
    close(fd);
}
void atomic_end() {
    // open share memory
    int fd = shm_open(atomic_name, O_RDWR, 0660);

    // release mutex
    pthread_mutex_t* mutex = (pthread_mutex_t*)mmap(NULL, sizeof(pthread_mutex_t), PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    pthread_mutex_unlock(mutex);
    munmap(mutex, sizeof(pthread_mutex_t));

    // close share memory
    close(fd);
}

#endif