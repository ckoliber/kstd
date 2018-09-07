#include <memory/low/Type.h>

#if defined(APP_WINDOWS)

#include <memory/low/Heap.h>
#include <stdarg.h>

struct String_ {
    struct String self;
    char* string;
};

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

void string_swap(char* char1, char* char2);

// implement methods
void string_swap(char* char1, char* char2) {
    if (*char1 != *char2) {
        *char1 ^= *char2;
        *char2 ^= *char1;
        *char1 ^= *char2;
    }
}

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
String* string_new_printf(char* format, ...) {
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
    wvsprintf(string_->string, format, args);
    va_end(args);

    return (String*)string_;
}
String* string_new_lower(char* value) {
    // init new string then lower
    String* string = string_new_printf("%s", value);
    string->lower(string);

    return string;
}
String* string_new_upper(char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->upper(string);

    return string;
}
String* string_new_reverse(char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->reverse(string);

    return string;
}
String* string_new_copy(char* value) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->copy(string, value);

    return string;
}
String* string_new_concat(char* value, char* data) {
    // init new string then upper
    String* string = string_new_printf("%s", value);
    string->concat(string, data);

    return string;
}
void string_free(String* string) {
    struct String_* string_ = (struct String_*)string;

    // destroy internal Char*
    heap_free(string_->string);

    heap_free(string_);
}

#endif