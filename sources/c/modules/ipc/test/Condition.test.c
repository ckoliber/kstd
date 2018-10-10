#include <low/Condition.h>

#include <assert.h>

int main() {
    Condition* condition = condition_new_object(NULL);

    condition_free(condition);
}