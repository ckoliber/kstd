#if defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#error Windows_OS
#else

#include <float.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if !defined(NULL)
#define NULL ((void*)0)
#endif

/*----------------------------------------------*/
/*----------------------------------------------*/
/*-----------Normal Signed and Unsigned---------*/
/*----------------------------------------------*/
/*----------------------------------------------*/
// signed fixed size 8, 16, 32, 64
typedef int8_t SignedByte;
typedef int16_t SignedShort;
typedef int32_t SignedInt;
typedef int64_t SignedLong;
#define SIGNED_BYTE_MAX INT8_MAX
#define SIGNED_BYTE_MIN INT8_MIN
#define SIGNED_SHORT_MAX INT16_MAX
#define SIGNED_SHORT_MIN INT16_MIN
#define SIGNED_INT_MAX INT32_MAX
#define SIGNED_INT_MIN INT32_MIN
#define SIGNED_LONG_MAX INT64_MAX
#define SIGNED_LONG_MIN INT64_MIN

// unsigned fixed size 8, 16, 32, 64
typedef uint8_t UnsignedByte;
typedef uint16_t UnsignedShort;
typedef uint32_t UnsignedInt;
typedef uint64_t UnsignedLong;
#define UNSIGNED_BYTE_MAX UINT8_MAX
#define UNSIGNED_BYTE_MIN 0
#define UNSIGNED_SHORT_MAX UINT16_MAX
#define UNSIGNED_SHORT_MIN 0
#define UNSIGNED_INT_MAX UINT32_MAX
#define UNSIGNED_INT_MIN 0
#define UNSIGNED_LONG_MAX UINT64_MAX
#define UNSIGNED_LONG_MIN 0

/*----------------------------------------------*/
/*----------------------------------------------*/
/*------------Fast Signed and Unsigned----------*/
/*----------------------------------------------*/
/*----------------------------------------------*/
// fast signed fixed size 8, 16, 32, 64
typedef int_fast8_t FastSignedByte;
typedef int_fast16_t FastSignedShort;
typedef int_fast32_t FastSignedInt;
typedef int_fast64_t FastSignedLong;
#define FAST_SIGNED_BYTE_MAX INT_FAST8_MAX
#define FAST_SIGNED_BYTE_MIN INT_FAST8_MIN
#define FAST_SIGNED_SHORT_MAX INT_FAST16_MAX
#define FAST_SIGNED_SHORT_MIN INT_FAST16_MIN
#define FAST_SIGNED_INT_MAX INT_FAST32_MAX
#define FAST_SIGNED_INT_MIN INT_FAST32_MIN
#define FAST_SIGNED_LONG_MAX INT_FAST64_MAX
#define FAST_SIGNED_LONG_MIN INT_FAST64_MIN

// fast unsigned fixed size 8, 16, 32, 64
typedef uint_fast8_t FastUnsignedByte;
typedef uint_fast16_t FastUnsignedShort;
typedef uint_fast32_t FastUnsignedInt;
typedef uint_fast64_t FastUnsignedLong;
#define FAST_UNSIGNED_BYTE_MAX UINT_FAST8_MAX
#define FAST_UNSIGNED_BYTE_MIN 0
#define FAST_UNSIGNED_SHORT_MAX UINT_FAST16_MAX
#define FAST_UNSIGNED_SHORT_MIN 0
#define FAST_UNSIGNED_INT_MAX UINT_FAST32_MAX
#define FAST_UNSIGNED_INT_MIN 0
#define FAST_UNSIGNED_LONG_MAX UINT_FAST64_MAX
#define FAST_UNSIGNED_LONG_MIN 0

/*----------------------------------------------*/
/*----------------------------------------------*/
/*-----------Least Signed and Unsigned----------*/
/*----------------------------------------------*/
/*----------------------------------------------*/
// signed least size 8, 16, 32, 64
typedef int_least8_t LeastSignedByte;
typedef int_least16_t LeastSignedShort;
typedef int_least32_t LeastSignedInt;
typedef int_least64_t LeastSignedLong;
#define LEAST_SIGNED_BYTE_MAX INT_LEAST8_MAX
#define LEAST_SIGNED_BYTE_MIN INT_LEAST8_MIN
#define LEAST_SIGNED_SHORT_MAX INT_LEAST16_MAX
#define LEAST_SIGNED_SHORT_MIN INT_LEAST16_MIN
#define LEAST_SIGNED_INT_MAX INT_LEAST32_MAX
#define LEAST_SIGNED_INT_MIN INT_LEAST32_MIN
#define LEAST_SIGNED_LONG_MAX INT_LEAST64_MAX
#define LEAST_SIGNED_LONG_MIN INT_LEAST64_MIN

// unsigned least size 8, 16, 32, 64
typedef uint_least8_t LeastUnsignedByte;
typedef uint_least16_t LeastUnsignedShort;
typedef uint_least32_t LeastUnsignedInt;
typedef uint_least64_t LeastUnsignedLong;
#define LEAST_UNSIGNED_BYTE_MAX UINT_LEAST8_MAX
#define LEAST_UNSIGNED_BYTE_MIN 0
#define LEAST_UNSIGNED_SHORT_MAX UINT_LEAST16_MAX
#define LEAST_UNSIGNED_SHORT_MIN 0
#define LEAST_UNSIGNED_INT_MAX UINT_LEAST32_MAX
#define LEAST_UNSIGNED_INT_MIN 0
#define LEAST_UNSIGNED_LONG_MAX UINT_LEAST64_MAX
#define LEAST_UNSIGNED_LONG_MIN 0

/*-------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*/
/*---------Void and Boolean and Size and Float and Double and LongDouble and String----------*/
/*-------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------*/
typedef void Void;
typedef bool Bool;
typedef size_t Size;
typedef float Float;
typedef double Double;
typedef long double LongDouble;
typedef char* String;

#endif

// string functions