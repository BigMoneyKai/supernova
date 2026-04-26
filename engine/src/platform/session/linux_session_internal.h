#pragma once

#if SN_PLATFORM_LINUX

#include "platform/platform.h"

#include <xcb/xcb.h>
#include <xkbcommon/xkbcommon.h>

typedef struct internal_state {
    xcb_connection_t* connection;
    xcb_window_t window;
    xcb_screen_t* screen;
    xcb_atom_t wm_protocols;
    xcb_atom_t wm_delete_win;

    platform_event_callbacks callbacks;

    struct xkb_context* xkb_context;
    struct xkb_keymap* xkb_keymap;
    struct xkb_state* xkb_state;
} internal_state;

#endif
