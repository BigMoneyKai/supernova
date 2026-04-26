#include "platform/platform.h"

#if SN_PLATFORM_APPLE

#include <signal.h>
#include <stdlib.h>

#include "core/event.h"
#include "platform/session/macos_session_internal.h"

static internal_state* mac_state = NULL;

static void mac_signal_handler(i32 signum) {
    if (signum == SIGINT && mac_state) {
        mac_state->quit_requested = SN_TRUE;
    }
}

b8 platform_startup(
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height
) {
    (void)application_name;
    (void)x;
    (void)y;
    (void)width;
    (void)height;

    plat_state->internal_state = malloc(sizeof(internal_state));
    if (!plat_state->internal_state) {
        return SN_FALSE;
    }

    internal_state* state = (internal_state*)plat_state->internal_state;
    platform_zero_memory(state, sizeof(internal_state));
    state->is_running = SN_TRUE;
    state->quit_requested = SN_FALSE;
    mac_state = state;
    signal(SIGINT, mac_signal_handler);
    return SN_TRUE;
}

void platform_shutdown(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (!state) {
        return;
    }

    free(state);
    plat_state->internal_state = NULL;
    mac_state = NULL;
}

b8 platform_pump_messages(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (!state || !state->is_running) {
        return SN_FALSE;
    }

    if (state->quit_requested) {
        event_context ctx = {};
        event_fire(EVENT_CODE_APP_QUIT, NULL, ctx);
        if (state->callbacks.on_quit) {
            state->callbacks.on_quit(state->callbacks.user_data);
        }
        state->is_running = SN_FALSE;
        return SN_FALSE;
    }

    platform_sleep(1);
    return SN_TRUE;
}

#endif
