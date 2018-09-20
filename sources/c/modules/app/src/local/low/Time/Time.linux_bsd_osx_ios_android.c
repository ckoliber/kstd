#include <local/low/Time.h>

#if defined(APP_LINUX)

#include <stdlib.h>
#include <sys/time.h>

uint_64 time_epochmillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}

#endif