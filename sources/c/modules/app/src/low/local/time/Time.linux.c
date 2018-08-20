#ifdef __unix__

#include <low/local/time/Time.h>

#include <stdint.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

uint64_t time_micro() {
    struct timeval time = {0, 0};
    gettimeofday(&time, NULL);
    return time.tv_sec * (uint64_t)1.0e6 + time.tv_usec;
}

#endif