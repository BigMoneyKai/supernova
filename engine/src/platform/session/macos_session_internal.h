#pragma once

#if SN_PLATFORM_APPLE

#include "platform/platform.h"

typedef struct internal_state {
    b8 is_running;
    b8 quit_requested;
    platform_event_callbacks callbacks;
} internal_state;

#endif
