#include <low/Date.h>
#include <kstd.h>

#include <assert.h>

void test_date_get_epoch();

void test_date_get_epoch(){
    assert(date_get_epoch() > 0);
}

int main() {
    kstd_init();

    for(int a = 0 ; a < 1000 ; a++){
        test_date_get_epoch();
    }
}
