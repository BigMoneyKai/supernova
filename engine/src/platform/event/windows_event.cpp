#include "platform/platform.h"

#if SN_PLATFORM_WINDOWS

#include "platform/session/windows_session_internal.h"

void platform_set_event_callbacks(
    platform_state* plat_state,
    const platform_event_callbacks* cb
) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (!state) {
        return;
    }

    if (cb) {
        state->callbacks = *cb;
    } else {
        platform_zero_memory(&state->callbacks, sizeof(platform_event_callbacks));
    }
}

#endif
