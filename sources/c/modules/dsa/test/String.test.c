#include <kstd.h>
#include <low/String.h>

#include <assert.h>

int main() {
    kstd_init();

    // string new test
    String* string_1 = string_new_printf("%d - %f", 1, 2.6);
    String* string_2 = string_new_lower("SaLaM");
    String* string_3 = string_new_upper("sAlAm");
    String* string_4 = string_new_reverse("SALAM");
    String* string_5 = string_new_copy("salam");
    String* string_6 = string_new_concat("sal", "am");
    String* string_7 = string_new_cut("SaLaMaa", 0, 5);
    String* string_8 = string_new_replace("saX", 2, 3, "lam");
    String* string_9 = string_new_copy("1243123214");
    String* string_10 = string_new_copy("124.32");

    assert(string_get_compare(string_1->vtable->value(string_1), "1 - 2.600000") == 0);
    assert(string_get_compare(string_2->vtable->value(string_2), "salam") == 0);
    assert(string_get_compare(string_3->vtable->value(string_3), "SALAM") == 0);
    assert(string_get_compare(string_4->vtable->value(string_4), "MALAS") == 0);
    assert(string_get_compare(string_5->vtable->value(string_5), "salam") == 0);
    assert(string_get_compare(string_6->vtable->value(string_6), "salam") == 0);
    assert(string_get_compare(string_7->vtable->value(string_7), "SaLaM") == 0);
    assert(string_get_compare(string_8->vtable->value(string_8), "salam") == 0);
    assert(string_9->vtable->to_long(string_9, 10) == 1243123214);
    assert(string_10->vtable->to_double(string_10) == 124.32);

    string_free(string_1);
    string_free(string_2);
    string_free(string_3);
    string_free(string_4);
    string_free(string_5);
    string_free(string_6);
    string_free(string_7);
    string_free(string_8);
    string_free(string_9);
    string_free(string_10);
}