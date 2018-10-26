#include <low/String.h>

#include <kstd.h>
#include <low/Heap.h>

#include <assert.h>

void test_string_printf();
void test_string_lower();
void test_string_upper();
void test_string_reverse();
void test_string_copy();
void test_string_concat();
void test_string_cut();
void test_string_replace();
void test_string_to_int();
void test_string_to_double();

void test_string_printf() {
    String* string = string_new_printf("%d - %f", 1, 2.6);

    assert(string_get_compare(string->vtable->value(string), "1 - 2.600000") == 0);

    string_free(string);
}
void test_string_lower() {
    String* string = string_new_lower("SaLaM");

    assert(string_get_compare(string->vtable->value(string), "salam") == 0);

    string_free(string);
}
void test_string_upper() {
    String* string = string_new_upper("sAlAm");

    assert(string_get_compare(string->vtable->value(string), "SALAM") == 0);

    string_free(string);
}
void test_string_reverse() {
    String* string = string_new_reverse("SALAM");

    assert(string_get_compare(string->vtable->value(string), "MALAS") == 0);

    string_free(string);
}
void test_string_copy() {
    String* string = string_new_copy("salam");

    assert(string_get_compare(string->vtable->value(string), "salam") == 0);

    string_free(string);
}
void test_string_concat() {
    String* string = string_new_concat("sal", "am");

    assert(string_get_compare(string->vtable->value(string), "salam") == 0);

    string_free(string);
}
void test_string_cut() {
    String* string = string_new_cut("SaLaMaa", 0, 5);

    assert(string_get_compare(string->vtable->value(string), "SaLaM") == 0);

    string_free(string);
}
void test_string_replace() {
    String* string = string_new_replace("saX", 2, 1, "lam");

    assert(string_get_compare(string->vtable->value(string), "salam") == 0);

    string_free(string);
}
void test_string_to_int() {
    String* string = string_new_copy("1243123214");

    assert(string->vtable->to_long(string, 10) == 1243123214);

    string_free(string);
}
void test_string_to_double() {
    String* string = string_new_copy("124.32");

    assert(string->vtable->to_double(string) == 124.32);

    string_free(string);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        test_string_printf();
        test_string_lower();
        test_string_upper();
        test_string_reverse();
        test_string_copy();
        test_string_concat();
        test_string_cut();
        test_string_replace();
        test_string_to_int();
        test_string_to_double();
    }
}