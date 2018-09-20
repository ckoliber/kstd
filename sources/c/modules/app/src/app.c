#include <app.h>

#include <ipc/low/Mutex.h>

#if defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_WINDOWS)

void app_init() {
    critical = mutex_new(1, "/critical");
}

#elif defined(APP_ANDROID)

void app_init() {
    critical = mutex_new(1, NULL);
}

#endif