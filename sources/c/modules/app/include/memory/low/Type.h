#include <app.h>

#if defined(APP_WINDOWS)

#include <windows.h>

// signed fixed size 8, 16, 32, 64
typedef __int8 SignedByte;
typedef __int16 SignedShort;
typedef __int32 SignedInt;
typedef __int64 SignedLong;
#define SIGNED_BYTE_MAX CHAR_MAX
#define SIGNED_BYTE_MIN CHAR_MIN
#define SIGNED_SHORT_MAX SHRT_MAX
#define SIGNED_SHORT_MIN SHRT_MIN
#define SIGNED_INT_MAX INT_MAX
#define SIGNED_INT_MIN INT_MIN
#define SIGNED_LONG_MAX _I64_MAX
#define SIGNED_LONG_MIN _I64_MIN

// unsigned fixed size 8, 16, 32, 64
typedef unsigned __int8 UnsignedByte;
typedef unsigned __int16 UnsignedShort;
typedef unsigned __int32 UnsignedInt;
typedef unsigned __int64 UnsignedLong;
#define UNSIGNED_BYTE_MAX UCHAR_MAX
#define UNSIGNED_BYTE_MIN 0
#define UNSIGNED_SHORT_MAX USHRT_MAX
#define UNSIGNED_SHORT_MIN 0
#define UNSIGNED_INT_MAX UINT_MAX
#define UNSIGNED_INT_MIN 0
#define UNSIGNED_LONG_MAX _UI64_MAX
#define UNSIGNED_LONG_MIN 0

typedef void Void;
typedef char Bool;
typedef char Char;
typedef SIZE_T Size;
typedef float Float;
typedef double Double;
typedef long double LongDouble;
#define TRUE 1
#define FALSE 0

#else

#include <float.h>
#include <stddef.h>
#include <stdint.h>

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

typedef void Void;
typedef char Bool;
typedef char Char;
typedef size_t Size;
typedef float Float;
typedef double Double;
typedef long double LongDouble;
#define TRUE 1
#define FALSE 0

#endif

// string functions
typedef struct String {
    // convert operators
    SignedInt (*to_int)(struct String* self);
    SignedLong (*to_long)(struct String* self);
    Double (*to_double)(struct String* self);

    // change value operators
    Void (*lower)(struct String* self);
    Void (*upper)(struct String* self);
    Void (*reverse)(struct String* self);
    Void (*copy)(struct String* self, Char* data);
    Void (*concat)(struct String* self, Char* data);

    // information operators
    Size (*length)(struct String* self);
    SignedInt (*compare)(struct String* self, Char* data);
    Char* (*value)(struct String* self);
} String;

String* string_new_printf(Char* format, ...);
String* string_new_lower(Char* value);
String* string_new_upper(Char* value);
String* string_new_reverse(Char* value);
String* string_new_copy(Char* value);
String* string_new_concat(Char* value, Char* data);
Void string_free(String* string);