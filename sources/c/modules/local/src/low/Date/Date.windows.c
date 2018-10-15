#include <low/Date.h>

#if defined(APP_WINDOWS)

// static
uint_64 date_get_epoch() {
    // time variables
    uint_64 epoch = ((uint_64)116444736000000000ULL);
    SYSTEMTIME system_time;
    FILETIME file_time;
    uint64_t time;

    // get epoch microtime
    GetSystemTime(&system_time);
    SystemTimeToFileTime(&system_time, &file_time);

    // get seconds
    time = ((uint64_t)file_time.dwLowDateTime);
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    return (uint_64) ((time - EPOCH) / 10000000L) * 1e6 + (system_time.wMilliseconds * 1000);
}

#endif