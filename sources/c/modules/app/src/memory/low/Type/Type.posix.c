#include <memory/low/Type.h>

#if !defined(APP_WINDOWS)

#include <ctype.h>
#include <memory/low/Heap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct String_ {
    struct String self;
    Char* string;
};

// link methods
// convert operators
SignedByte string_to_byte(struct String* self);
SignedShort string_to_short(struct String* self);
SignedInt string_to_int(struct String* self);
SignedLong string_to_long(struct String* self);
Float string_to_float(struct String* self);
Double string_to_double(struct String* self);
LongDouble string_to_longdouble(struct String* self);

// change value operators
SignedInt string_lower(struct String* self);
SignedInt string_upper(struct String* self);
SignedInt string_reverse(struct String* self);
SignedInt string_concat(struct String* self, Char* data);
SignedInt string_set(struct String* self, UnsignedInt from, UnsignedInt count, Char data);
SignedInt string_copy(struct String* self, UnsignedInt from, UnsignedInt count, Char* data);

// information operators
SignedInt string_length(struct String* self);
SignedInt string_compare(struct String* self, Char* data);
Char* string_value(struct String* self);

// regex operators
// Bool string_match(Char* regex);
// Void string_replace(Char* regex, UnsignedInt count, Char* data);
// struct ArrayList string_find(Char* regex, UnsignedInt count);
// struct ArrayList string_export(Char* regex);
// Void string_import(Char* regex, ArrayList data);

Void string_swap(Char* char1, Char* char2);

// implement methods
Void string_swap(Char* char1, Char* char2) {
    if (*char1 != *char2) {
        *char1 ^= *char2;
        *char2 ^= *char1;
        *char1 ^= *char2;
    }
}

SignedByte string_to_byte(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    strto

        // convert string to int to SignedByte
        int value = atoi(string_->string);
    SignedByte result = (SignedByte)value;

    return result;
}
SignedShort string_to_short(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to int to SignedByte
    int value = atoi(string_->string);
    SignedShort result = (SignedShort)value;

    return result;
}
SignedInt string_to_int(struct String* self) {}
SignedLong string_to_long(struct String* self) {}
Float string_to_float(struct String* self) {}
Double string_to_double(struct String* self) {}
LongDouble string_to_longdouble(struct String* self) {}
SignedInt string_lower(struct String* self) {}
SignedInt string_upper(struct String* self) {}
SignedInt string_reverse(struct String* self) {}
SignedInt string_concat(struct String* self, Char* data) {}
SignedInt string_set(struct String* self, UnsignedInt from, UnsignedInt count, Char data) {}
SignedInt string_copy(struct String* self, UnsignedInt from, UnsignedInt count, Char* data) {}
SignedInt string_length(struct String* self) {}
SignedInt string_compare(struct String* self, Char* data) {}
Char* string_value(struct String* self) {}

String* string_new(Char* value) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init internal Char*
    string_->string = heap_alloc(strlen(value) + 1);
    strcpy(string_->string, value);

    return (String*)string_;
}
String* string_new_printf(Char* format, ...) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init internal Char*
    va_list args;
    va_start(args, format);
    string_->string = heap_alloc(vsnprintf(NULL, 0, format, args) + 1);
    vsprintf(string_->string, format, args);
    va_end(args);

    return (String*)string_;
}
String* string_new_lower(Char* value) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init internal Char*
    string_->string = heap_alloc(strlen(value) + 1);
    strcpy(string_->string, value);
    for (int cursor = 0; cursor < strlen(value); cursor++) {
        string_->string[cursor] = tolower(string_->string[cursor]);
    }

    return (String*)string_;
}
String* string_new_upper(Char* value) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init internal Char*
    string_->string = heap_alloc(strlen(value) + 1);
    strcpy(string_->string, value);
    for (int cursor = 0; cursor < strlen(value); cursor++) {
        string_->string[cursor] = toupper(string_->string[cursor]);
    }

    return (String*)string_;
}
String* string_new_reverse(Char* value) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init internal Char*
    string_->string = heap_alloc(strlen(value) + 1);
    strcpy(string_->string, value);
    for (int cursor = 0; cursor <= strlen(value) / 2; cursor++) {
        string_swap((string_->string + cursor), (string_->string + (strlen(value) - 1) - cursor));
    }

    return (String*)string_;
}
String* string_new_concat(Char* value, Char* data) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init internal Char*
    string_->string = heap_alloc(strlen(value) + strlen(data) + 1);
    strcpy(string_->string, value);
    strcat(string_->string, data);

    return (String*)string_;
}
// String* string_new_replace(Char* value, Char* regex, UnsignedInt count, Char* data){}
// String* string_new_import(Char* value, Char* regex, ArrayList data){}
Void string_free(String* string) {
    struct String_* string_ = (struct String_*)string;

    // destroy internal Char*
    heap_free(string_->string);

    heap_free(string_);
}

#endif