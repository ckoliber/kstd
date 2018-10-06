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

#if defined(APP_WINDOWS)

#include <windows.h>

// signed fixed size 8, 16, 32, 64 integer
typedef __int8 int_8;
typedef __int16 int_16;
typedef __int32 int_32;
typedef __int64 int_64;
#define INT_8_MAX CHAR_MAX
#define INT_8_MIN CHAR_MIN
#define INT_16_MAX SHRT_MAX
#define INT_16_MIN SHRT_MIN
#define INT_32_MAX INT_MAX
#define INT_32_MIN INT_MIN
#define INT_64_MAX _I64_MAX
#define INT_64_MIN _I64_MIN

// unsigned fixed size 8, 16, 32, 64 integer
typedef unsigned __int8 uint_8;
typedef unsigned __int16 uint_16;
typedef unsigned __int32 uint_32;
typedef unsigned __int64 uint_64;
#define UINT_8_MAX UCHAR_MAX
#define UINT_8_MIN 0
#define UINT_16_MAX USHRT_MAX
#define UINT_16_MIN 0
#define UINT_32_MAX UINT_MAX
#define UINT_32_MIN 0
#define UINT_64_MAX _UI64_MAX
#define UINT_64_MIN 0

// (short, long)(signed, unsigned) void, bool, char, size, float, double
typedef SIZE_T tsize;
typedef char bool;
#define true 1
#define false 0

#else

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

// [const][short, long][signed, unsigned] void, bool, char, size, float, double
typedef size_t tsize;
typedef char bool;
#define true 1
#define false 0

#endif