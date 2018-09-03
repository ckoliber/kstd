#include <low/local/Time.h>

#include <stdlib.h>
#include <sys/time.h>

long int time_epochmillis() {
    struct timeval time;
    gettimeofday(&time, NULL);
    return time.tv_sec * 1000 + time.tv_usec / 1000;
}