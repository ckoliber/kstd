#if defined(_WIN32) || defined(_WIN64)
    #define APP_WINDOWS
    #if defined(_MSC_VER)
        #define APP_WINDOWS_MSVC
    #elif defined(__MINGW32__) || defined(__MINGW64__)
        #define APP_WINDOWS_MINGW
    #endif
#elif defined(__APPLE__) || defined(__MACH__)
    #include <TargetConditionals.h>
    #if TARGET_OS_MAC == 1
        #define APP_OSX
    #elif TARGET_IPHONE_SIMULATOR == 1
        #define APP_IOS
    #elif TARGET_OS_IPHONE == 1
        #define APP_IOS
    #endif
#elif defined(__linux__)  // linux
    #define _GNU_SOURCE
    #include <sys/param.h>
    #if defined(BSD)
        #define APP_BSD
    #elif defined(__ANDROID__)
        #define APP_ANDROID
    #else
        #define APP_LINUX
    #endif
#endif

#if defined(APP_WINDOWS)

#include <windows.h>
#include <float.h>
#include <stddef.h>
#include <stdint.h>

// signed fixed size 8, 16, 32, 64 integer
typedef __int8 int_8;
typedef __int16 int_16;
typedef __int32 int_32;
typedef __int64 int_64;
#define INT_8_MAX INT8_MAX
#define INT_8_MIN INT8_MIN
#define INT_16_MAX INT16_MAX
#define INT_16_MIN INT16_MIN
#define INT_32_MAX INT32_MAX
#define INT_32_MIN INT32_MIN
#define INT_64_MAX INT64_MAX
#define INT_64_MIN INT64_MIN

// unsigned fixed size 8, 16, 32, 64 integer
typedef unsigned __int8 uint_8;
typedef unsigned __int16 uint_16;
typedef unsigned __int32 uint_32;
typedef unsigned __int64 uint_64;
#define UINT_8_MAX UINT8_MAX
#define UINT_8_MIN 0
#define UINT_16_MAX UINT16_MAX
#define UINT_16_MIN 0
#define UINT_32_MAX UINT32_MAX
#define UINT_32_MIN 0
#define UINT_64_MAX UINT64_MAX
#define UINT_64_MIN 0

// (short, long)(signed, unsigned) void, bool, char, tsize, float, double
typedef SIZE_T tsize;
typedef int bool;
#define true 1
#define false 0

#elif defined(APP_LINUX) || defined(APP_BSD) || defined(APP_OSX) || defined(APP_IOS) || defined(APP_ANDROID)

#include <float.h>
#include <stddef.h>
#include <stdint.h>

// signed fixed size 8, 16, 32, 64 integer
typedef int8_t int_8;
typedef int16_t int_16;
typedef int32_t int_32;
typedef int64_t int_64;
#define INT_8_MAX INT8_MAX
#define INT_8_MIN INT8_MIN
#define INT_16_MAX INT16_MAX
#define INT_16_MIN INT16_MIN
#define INT_32_MAX INT32_MAX
#define INT_32_MIN INT32_MIN
#define INT_64_MAX INT64_MAX
#define INT_64_MIN INT64_MIN

// unsigned fixed size 8, 16, 32, 64 integer
typedef uint8_t uint_8;
typedef uint16_t uint_16;
typedef uint32_t uint_32;
typedef uint64_t uint_64;
#define UINT_8_MAX UINT8_MAX
#define UINT_8_MIN 0
#define UINT_16_MAX UINT16_MAX
#define UINT_16_MIN 0
#define UINT_32_MAX UINT32_MAX
#define UINT_32_MIN 0
#define UINT_64_MAX UINT64_MAX
#define UINT_64_MIN 0

// [const][short, long][signed, unsigned] void, bool, char, tsize, float, double
typedef size_t tsize;
typedef int bool;
#define true 1
#define false 0

#endif
