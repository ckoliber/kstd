#include <stdio.h>

#define PT_SUITE(name) void name(void)

#define PT_FUNC(name) static void name(void)
#define PT_TEST(name)     \
    auto void name(void); \
    void name(void)

// PT_SUITE(suite_basic) {
//     PT_TEST(test_maths) {
//         int x = 3;
//         int y = 11;

//         PT_ASSERT(1 + 1 == 2);
//         PT_ASSERT(2 + 2 == 4);
//         PT_ASSERT(y - x == 8);
//     }

//     PT_TEST(test_strings) {
//         PT_ASSERT(strcmp("x", "x") == 0);
//         PT_ASSERT_STR_EQ("x", "x");
//     }
// }

#include <stddef.h>
#include <stdlib.h>

struct parent {
}
#define NULL_PARENT parent

#define PRIVATE_CLASS(name, parent, body) \
    struct name {                         \
        struct parent;                    \
    };

// #define CALL(struct, method)      \
//     if (struct->method != NULL) { \
//         struct->method();         \
//     } else {                      \
//         printf("NOT");            \
//     }

//     struct Test {
//     void (*a)();
// };

int main(int argc, char* argv[]) {
    auto int test();
}