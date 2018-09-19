#include <app.h>

#include <ipc/low/Mutex.h>

void app_init() {
    critical = mutex_new(1, "/critical");
}