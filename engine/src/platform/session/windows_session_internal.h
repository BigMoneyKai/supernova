#pragma once

#if SN_PLATFORM_WINDOWS

#include "platform/platform.h"

#include <windows.h>

typedef struct internal_state {
    HINSTANCE h_instance;
    HWND hwnd;
    b8 is_running;
    platform_event_callbacks callbacks;
} internal_state;

#endif
