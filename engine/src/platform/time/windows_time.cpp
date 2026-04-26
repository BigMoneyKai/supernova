#include "platform/platform.h"

#if SN_PLATFORM_WINDOWS

#include <windows.h>

f64 platform_get_absolute_time() {
    static LARGE_INTEGER freq;
    static b8 initialized = SN_FALSE;

    if (!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = SN_TRUE;
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    return (f64)now.QuadPart / (f64)freq.QuadPart;
}

void platform_sleep(u64 ms) {
    if (ms > 0xFFFFFFFFULL) {
        ms = 0xFFFFFFFFULL;
    }
    Sleep((DWORD)ms);
}

#endif
