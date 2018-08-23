#include <low/processor/low/Process.h>

#include <assert.h>

int main(int argc, char* argv[]) {
    Process* p1 = process_new();
    Process* p2 = process_new();

    p1->start(p1, "sleep 3");
    p2->start(p2, "ls -altrh");

    p1->join(p1);
    p2->join(p2);

    p1->stop(p1);
    p2->stop(p2);

    process_free(p1);
    process_free(p2);

    return 0;
}