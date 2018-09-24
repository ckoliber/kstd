#include <memory/low/Type.h>

// vtable
typedef struct String_VTable {
    // convert operators
    int (*to_int)(struct String* self);
    long (*to_long)(struct String* self);
    double (*to_double)(struct String* self);

    // change value operators
    void (*lower)(struct String* self);
    void (*upper)(struct String* self);
    void (*reverse)(struct String* self);
    void (*copy)(struct String* self, char* data);
    void (*concat)(struct String* self, char* data);
    void (*cut)(struct String* self, int begin, int end);

    // information operators
    tsize (*length)(struct String* self);
    int (*compare)(struct String* self, char* data);
    char* (*value)(struct String* self);
} String_VTable;

// vtable + private data problem solve
typedef struct String {
    String_VTable* vtable;
} String;

// init vtable
void string_init();

// new raw string
String* string_new();

// free raw string
void string_free(String* string);

// new string
String* string_new_printf(char* format, ...);
String* string_new_lower(char* value);
String* string_new_upper(char* value);
String* string_new_reverse(char* value);
String* string_new_copy(char* value);
String* string_new_concat(char* value, char* data);
String* string_new_cut(char* value, int begin, int end);