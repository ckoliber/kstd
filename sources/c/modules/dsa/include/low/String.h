#include <low/Type.h>

// structs
typedef struct String String;
typedef struct String_VTable String_VTable;

// implement structs
// vtable + private data problem solve
struct String {
    String_VTable* vtable;
};

// vtable
struct String_VTable {
    // convert operators
    long (*to_long)(String* self, int base);
    double (*to_double)(String* self);

    // change value operators
    void (*lower)(String* self);
    void (*upper)(String* self);
    void (*reverse)(String* self);
    void (*copy)(String* self, char* data);
    void (*concat)(String* self, char* data);
    void (*cut)(String* self, int begin, int end);
    void (*replace)(String* self, int begin, int end, char* replace);

    // information operators
    tsize (*length)(String* self);
    int (*compare)(String* self, char* data);
    char* (*value)(String* self);
};

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
String* string_new_replace(char* value, int begin, int end, char* replace);

// local string methods
tsize string_get_length(char* value);
int string_get_compare(char* value, char* data);
