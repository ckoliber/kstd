#include <dsa/low/String.h>

#if defined(APP_WINDOWS)
#include <ctype.h>
#include <memory/low/Heap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct String_ {
    // self public object
    String self;

    // constructor data

    // private data
    char* string;
};

// vtable
String_VTable* string_vtable;

// link methods
int string_to_int(struct String* self);
long string_to_long(struct String* self);
double string_to_double(struct String* self);
void string_lower(struct String* self);
void string_upper(struct String* self);
void string_reverse(struct String* self);
void string_copy(struct String* self, char* data);
void string_concat(struct String* self, char* data);
tsize string_length(struct String* self);
int string_compare(struct String* self, char* data);
char* string_value(struct String* self);

// local methods
void string_swap(char* char1, char* char2);

// implement methods
void string_swap(char* char1, char* char2) {
    if (*char1 != *char2) {
        *char1 ^= *char2;
        *char2 ^= *char1;
        *char1 ^= *char2;
    }
}

// vtable operators
int string_to_int(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to SignedInt
    int result = atoi(string_->string);

    return result;
}
long string_to_long(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to SignedLong
    long result = atoll(string_->string);

    return result;
}
double string_to_double(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to Double
    double result = atof(string_->string);

    return result;
}
void string_lower(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert all Char's to lower
    string_->string = CharLower(string_->string);
}
void string_upper(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert all Char's to upper
    string_->string = CharUpper(string_->string);
}
void string_reverse(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // reverse string
    for (int cursor = 0; cursor <= lstrlen(string_->string) / 2; cursor++) {
        string_swap((string_->string + cursor), (string_->string + (lstrlen(string_->string) - 1) - cursor));
    }
}
void string_copy(struct String* self, char* data) {
    struct String_* string_ = (struct String_*)self;

    // copy data to string
    string_->string = heap_realloc(string_->string, lstrlen(data) + 1);
    lstrcpy(string_->string, data);
}
void string_concat(struct String* self, char* data) {
    struct String_* string_ = (struct String_*)self;

    // concatenate data to string
    string_->string = heap_realloc(string_->string, lstrlen(string_->string) + lstrlen(data) + 1);
    lstrcat(string_->string, data);
}
tsize string_length(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // compute string length
    tsize result = lstrlen(string_->string);

    return result;
}
int string_compare(struct String* self, char* data) {
    struct String_* string_ = (struct String_*)self;

    // compare string
    int result = lstrcmp(string_->string, data);

    return result;
}
char* string_value(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // get string
    char* result = string_->string;

    return result;
}

// object allocation and deallocation operators
void string_init() {
    // init vtable
    string_vtable = heap_alloc(sizeof(String_VTable));
    string_vtable->to_int = string_to_int;
    string_vtable->to_long = string_to_long;
    string_vtable->to_double = string_to_double;
    string_vtable->lower = string_lower;
    string_vtable->upper = string_upper;
    string_vtable->reverse = string_reverse;
    string_vtable->copy = string_copy;
    string_vtable->concat = string_concat;
    string_vtable->length = string_length;
    string_vtable->compare = string_compare;
    string_vtable->value = string_value;
}
String* string_new() {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // set vtable
    string_->self.vtable = string_vtable;

    // set constructor data

    // set private data
    string_->string = NULL;

    return (String*)string_;
}
void string_free(String* string) {
    struct String_* string_ = (struct String_*)string;

    // free private data
    if (string_->string != NULL) {
        heap_free(string_->string);
    }

    // free self
    heap_free(string_);
}
String* string_new_printf(char* format, ...) {
    struct String_* string_ = (struct String_*)string_new();

    // set constructor data

    // set private data
    va_list args;
    va_start(args, format);
    string_->string = heap_alloc(vsnprintf(NULL, 0, format, args) + 1);
    vsprintf(string_->string, format, args);
    va_end(args);

    return (String*)string_;
}
String* string_new_lower(char* value) {
    // init new string then lower
    String* string = string_new_printf("%s", value);
    string->vtable->lower(string);

    return string;
}
String* string_new_upper(char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->vtable->upper(string);

    return string;
}
String* string_new_reverse(char* value) {
    // init new string then reverse
    String* string = string_new_printf("%s", value);
    string->vtable->reverse(string);

    return string;
}
String* string_new_copy(char* value) {
    // init new string then copy
    String* string = string_new_printf("%s", value);

    return string;
}
String* string_new_concat(char* value, char* data) {
    // init new string then concat
    String* string = string_new_printf("%s", value);
    string->vtable->concat(string, data);

    return string;
}

#endif