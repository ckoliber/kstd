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
SignedInt string_to_int(struct String* self);
SignedLong string_to_long(struct String* self);
Double string_to_double(struct String* self);
Void string_lower(struct String* self);
Void string_upper(struct String* self);
Void string_reverse(struct String* self);
Void string_copy(struct String* self, Char* data);
Void string_concat(struct String* self, Char* data);
Size string_length(struct String* self);
SignedInt string_compare(struct String* self, Char* data);
Char* string_value(struct String* self);

Void string_swap(Char* char1, Char* char2);

// implement methods
Void string_swap(Char* char1, Char* char2) {
    if (*char1 != *char2) {
        *char1 ^= *char2;
        *char2 ^= *char1;
        *char1 ^= *char2;
    }
}

SignedInt string_to_int(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to SignedInt
    int value = atoi(string_->string);
    SignedInt result = (SignedInt)value;

    return result;
}
SignedLong string_to_long(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to SignedLong
    long long value = atoll(string_->string);
    SignedLong result = (SignedLong)value;

    return result;
}
Double string_to_double(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert string to Double
    double value = atof(string_->string);
    Double result = (Double)value;

    return result;
}
Void string_lower(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert all Char's to lower
    for (int cursor = 0; cursor < strlen(string_->string); cursor++) {
        string_->string[cursor] = tolower(string_->string[cursor]);
    }
}
Void string_upper(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // convert all Char's to upper
    for (int cursor = 0; cursor < strlen(string_->string); cursor++) {
        string_->string[cursor] = toupper(string_->string[cursor]);
    }
}
Void string_reverse(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // reverse string
    for (int cursor = 0; cursor <= strlen(string_->string) / 2; cursor++) {
        string_swap((string_->string + cursor), (string_->string + (strlen(string_->string) - 1) - cursor));
    }
}
Void string_copy(struct String* self, Char* data) {
    struct String_* string_ = (struct String_*)self;

    // copy data to string
    string_->string = heap_realloc(string_->string, strlen(data) + 1);
    strcpy(string_->string, data);
}
Void string_concat(struct String* self, Char* data) {
    struct String_* string_ = (struct String_*)self;

    // concatenate data to string
    string_->string = heap_realloc(string_->string, strlen(string_->string) + strlen(data) + 1);
    strcat(string_->string, data);
}
Size string_length(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // compute string length
    Size result = strlen(string_->string);

    return result;
}
SignedInt string_compare(struct String* self, Char* data) {
    struct String_* string_ = (struct String_*)self;

    // compare string
    SignedInt result = strcmp(string_->string, data);

    return result;
}
Char* string_value(struct String* self) {
    struct String_* string_ = (struct String_*)self;

    // get string
    Char* result = string_->string;

    return result;
}
String* string_new_printf(Char* format, ...) {
    struct String_* string_ = heap_alloc(sizeof(struct String_));

    // init private methods
    string_->self.to_int = string_to_int;
    string_->self.to_long = string_to_long;
    string_->self.to_double = string_to_double;
    string_->self.lower = string_lower;
    string_->self.upper = string_upper;
    string_->self.reverse = string_reverse;
    string_->self.copy = string_copy;
    string_->self.concat = string_concat;
    string_->self.length = string_length;
    string_->self.compare = string_compare;
    string_->self.value = string_value;

    // init internal Char*
    va_list args;
    va_start(args, format);
    string_->string = heap_alloc(vsnprintf(NULL, 0, format, args) + 1);
    vsprintf(string_->string, format, args);
    va_end(args);

    return (String*)string_;
}
String* string_new_lower(Char* value) {
    // init new string then lower
    String* string = string_new_printf("%s", value);
    string->lower(string);

    return string;
}
String* string_new_upper(Char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->upper(string);

    return string;
}
String* string_new_reverse(Char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->reverse(string);

    return string;
}
String* string_new_copy(Char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->copy(string, value);

    return string;
}
String* string_new_concat(Char* value, Char* data) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->concat(string, data);

    return string;
}
Void string_free(String* string) {
    struct String_* string_ = (struct String_*)string;

    // destroy internal Char*
    heap_free(string_->string);

    heap_free(string_);
}

#endif