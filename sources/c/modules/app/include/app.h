#define _GNU_SOURCE
#if defined(_WIN32) || defined(_WIN64)  // windows
#define APP_WINDOWS
#elif defined(__linux__)  // linux
#define APP_LINUX
#elif defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))  // osx or ios or bsd
#include <sys/param.h>
#if defined(BSD)
#define APP_BSD
#elif defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#if TARGET_IPHONE_SIMULATOR == 1
#define APP_IOS
#elif TARGET_OS_IPHONE == 1
#define APP_IOS
#elif TARGET_OS_MAC == 1
#define APP_OSX
#endif
#endif
#elif defined(__ANDROID__)  // android
#define APP_ANDROID
#endif

void app_init();