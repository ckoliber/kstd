#include <low/Date.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// static
uint_64 date_get_epoch() {
    // get epoch microtime
    struct timeval time;
    gettimeofday(&time, NULL);

    // get epoch millis time
    return (uint_64) (time.tv_sec * 1e6 + time.tv_usec);
}

#endif