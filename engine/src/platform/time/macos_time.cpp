#include "platform/platform.h"

#if SN_PLATFORM_APPLE

#include <time.h>
#include <unistd.h>

f64 platform_get_absolute_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (f64)ts.tv_sec + (f64)ts.tv_nsec * 1e-9;
}

void platform_sleep(u64 ms) {
    struct timespec ts;
    ts.tv_sec = (time_t)(ms / 1000);
    ts.tv_nsec = (long)((ms % 1000) * 1000000ULL);
    nanosleep(&ts, 0);
}

#endif
