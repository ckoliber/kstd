#include <low/Date.h>

#include <assert.h>

void test_date_get_epoch();

void test_date_get_epoch(){
    assert(date_get_epoch() > 0);
}

int main() {
    test_date_get_epoch();
}
