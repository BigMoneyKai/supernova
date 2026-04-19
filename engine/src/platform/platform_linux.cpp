#define _POSIX_C_SOURCE 200809L

#include "platform.h"
#if SN_PLATFORM_LINUX

#include "core/logger.h"
#include "core/event.h"

#include <xcb/xcb.h>
#include <xcb/xcb_icccm.h>
#include <xkbcommon/xkbcommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

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

static key_code xkb_keysym_to_key_code(xkb_keysym_t sym) {
    switch(sym) {
        case XKB_KEY_A: return KEY_CODE_A;
        case XKB_KEY_a: return KEY_CODE_a;
        case XKB_KEY_B: return KEY_CODE_B;
        case XKB_KEY_b: return KEY_CODE_b;
        case XKB_KEY_C: return KEY_CODE_C;
        case XKB_KEY_c: return KEY_CODE_c;
        case XKB_KEY_D: return KEY_CODE_D;
        case XKB_KEY_d: return KEY_CODE_d;
        case XKB_KEY_E: return KEY_CODE_E;
        case XKB_KEY_e: return KEY_CODE_e;
        case XKB_KEY_F: return KEY_CODE_F;
        case XKB_KEY_f: return KEY_CODE_f;
        case XKB_KEY_G: return KEY_CODE_G;
        case XKB_KEY_g: return KEY_CODE_g;
        case XKB_KEY_H: return KEY_CODE_H;
        case XKB_KEY_h: return KEY_CODE_h;
        case XKB_KEY_I: return KEY_CODE_I;
        case XKB_KEY_i: return KEY_CODE_i;
        case XKB_KEY_J: return KEY_CODE_J;
        case XKB_KEY_j: return KEY_CODE_j;
        case XKB_KEY_K: return KEY_CODE_K;
        case XKB_KEY_k: return KEY_CODE_k;
        case XKB_KEY_L: return KEY_CODE_L;
        case XKB_KEY_l: return KEY_CODE_l;
        case XKB_KEY_M: return KEY_CODE_M;
        case XKB_KEY_m: return KEY_CODE_m;
        case XKB_KEY_N: return KEY_CODE_N;
        case XKB_KEY_n: return KEY_CODE_n;
        case XKB_KEY_O: return KEY_CODE_O;
        case XKB_KEY_o: return KEY_CODE_o;
        case XKB_KEY_P: return KEY_CODE_P;
        case XKB_KEY_p: return KEY_CODE_p;
        case XKB_KEY_Q: return KEY_CODE_Q;
        case XKB_KEY_q: return KEY_CODE_q;
        case XKB_KEY_R: return KEY_CODE_R;
        case XKB_KEY_r: return KEY_CODE_r;
        case XKB_KEY_S: return KEY_CODE_S;
        case XKB_KEY_s: return KEY_CODE_s;
        case XKB_KEY_T: return KEY_CODE_T;
        case XKB_KEY_t: return KEY_CODE_t;
        case XKB_KEY_U: return KEY_CODE_U;
        case XKB_KEY_u: return KEY_CODE_u;
        case XKB_KEY_V: return KEY_CODE_V;
        case XKB_KEY_v: return KEY_CODE_v;
        case XKB_KEY_W: return KEY_CODE_W;
        case XKB_KEY_w: return KEY_CODE_w;
        case XKB_KEY_X: return KEY_CODE_X;
        case XKB_KEY_x: return KEY_CODE_x;
        case XKB_KEY_Y: return KEY_CODE_Y;
        case XKB_KEY_y: return KEY_CODE_y;
        case XKB_KEY_Z: return KEY_CODE_Z;
        case XKB_KEY_z: return KEY_CODE_z;

        case XKB_KEY_0: return KEY_CODE_0;
        case XKB_KEY_1: return KEY_CODE_1;
        case XKB_KEY_2: return KEY_CODE_2;
        case XKB_KEY_3: return KEY_CODE_3;
        case XKB_KEY_4: return KEY_CODE_4;
        case XKB_KEY_5: return KEY_CODE_5;
        case XKB_KEY_6: return KEY_CODE_6;
        case XKB_KEY_7: return KEY_CODE_7;
        case XKB_KEY_8: return KEY_CODE_8;
        case XKB_KEY_9: return KEY_CODE_9;

        case XKB_KEY_Up: return KEY_CODE_UP;
        case XKB_KEY_Down: return KEY_CODE_DOWN;
        case XKB_KEY_Left: return KEY_CODE_LEFT;
        case XKB_KEY_Right: return KEY_CODE_RIGHT;
        case XKB_KEY_Shift_L: return KEY_CODE_LSHIFT;
        case XKB_KEY_Shift_R: return KEY_CODE_RSHIFT;
        case XKB_KEY_Control_L: return KEY_CODE_LCTRL;
        case XKB_KEY_Control_R: return KEY_CODE_RCTRL;

        default: return KEY_CODE_UNKNOWN;
    }
}

static mouse_code xcb_button_to_mouse_code(u8 button) {
    switch(button) {
        case 1: return MOUSE_CODE_1; // left
        case 3: return MOUSE_CODE_2; // right
        case 2: return MOUSE_CODE_3; // middle
        case 8: return MOUSE_CODE_4; // back/extra
        default: return MOUSE_CODE_UNKNOWN;
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
    plat_state->internal_state = malloc(sizeof(internal_state));
    if(!plat_state->internal_state) {
        ERROR("Failed to allocate Linux platform state.");
        return SN_FALSE;
    }
    internal_state* state = (internal_state*)plat_state->internal_state;
    platform_zero_memory(state, sizeof(internal_state));

    state->connection = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(state->connection)) {
        ERROR("Failed to connect to X server.\n");
        free(state);
        plat_state->internal_state = NULL;
        return SN_FALSE;
    }

    const xcb_setup_t* setup = xcb_get_setup(state->connection);
    xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup);
    state->screen = it.data;

    state->window = xcb_generate_id(state->connection);

    u32 value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    u32 value_list[] = {
        state->screen->black_pixel,
        XCB_EVENT_MASK_EXPOSURE |
        XCB_EVENT_MASK_KEY_PRESS |
        XCB_EVENT_MASK_KEY_RELEASE |
        XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_BUTTON_RELEASE |
        XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_STRUCTURE_NOTIFY
    };

    xcb_create_window(
        state->connection,
        XCB_COPY_FROM_PARENT,
        state->window,
        state->screen->root,
        x, y,
        width, height,
        0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT,
        state->screen->root_visual,
        value_mask,
        value_list
    );

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        XCB_ATOM_WM_NAME,
        XCB_ATOM_STRING,
        8,
        strlen(application_name),
        application_name
    );

    xcb_intern_atom_cookie_t wm_delete_cookie =
        xcb_intern_atom(state->connection, 0, 16, "WM_DELETE_WINDOW");

    xcb_intern_atom_cookie_t wm_protocols_cookie =
        xcb_intern_atom(state->connection, 0, 12, "WM_PROTOCOLS");

    xcb_intern_atom_reply_t* wm_delete_reply =
        xcb_intern_atom_reply(state->connection, wm_delete_cookie, NULL);

    xcb_intern_atom_reply_t* wm_protocols_reply =
        xcb_intern_atom_reply(state->connection, wm_protocols_cookie, NULL);

    state->wm_delete_win = wm_delete_reply->atom;
    state->wm_protocols = wm_protocols_reply->atom;

    xcb_change_property(
        state->connection,
        XCB_PROP_MODE_REPLACE,
        state->window,
        state->wm_protocols,
        XCB_ATOM_ATOM,
        32,
        1,
        &state->wm_delete_win
    );

    free(wm_delete_reply);
    free(wm_protocols_reply);

    xcb_map_window(state->connection, state->window);

    xcb_flush(state->connection);

    state->xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    struct xkb_rule_names names{};
    if(!state->xkb_context) {
        ERROR("Failed to create XKB context.");
        goto xkb_fail;
    }
    state->xkb_keymap = xkb_keymap_new_from_names(state->xkb_context, &names,
                                                  XKB_KEYMAP_COMPILE_NO_FLAGS);
    if(!state->xkb_keymap) {
        ERROR("Failed to create XKB keymap.");
        goto xkb_fail;
    }
    state->xkb_state = xkb_state_new(state->xkb_keymap);
    if(!state->xkb_state) {
        ERROR("Failed to create XKB state.");
        goto xkb_fail;
    }

    return SN_TRUE;

xkb_fail:
    if (state->xkb_state) {
        xkb_state_unref(state->xkb_state);
        state->xkb_state = NULL;
    }
    if (state->xkb_keymap) {
        xkb_keymap_unref(state->xkb_keymap);
        state->xkb_keymap = NULL;
    }
    if (state->xkb_context) {
        xkb_context_unref(state->xkb_context);
        state->xkb_context = NULL;
    }
    xcb_destroy_window(state->connection, state->window);
    xcb_disconnect(state->connection);
    free(state);
    plat_state->internal_state = NULL;
    return SN_FALSE;
}

void platform_shutdown(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (!state) {
        return;
    }
    if (state->connection) {
        xcb_destroy_window(state->connection, state->window);
        xcb_disconnect(state->connection);
    }
    if (state->xkb_state) {
        xkb_state_unref(state->xkb_state);
    }
    if (state->xkb_keymap) {
        xkb_keymap_unref(state->xkb_keymap);
    }
    if (state->xkb_context) {
        xkb_context_unref(state->xkb_context);
    }
    free(state);
    plat_state->internal_state = NULL;
}

b8 platform_pump_messages(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    xcb_generic_event_t* event;

    if (xcb_connection_has_error(state->connection)) {
        return SN_FALSE;
    }

    while ((event = xcb_poll_for_event(state->connection))) {
        u8 type = event->response_type & ~0x80;

        switch (type) {
            case XCB_CLIENT_MESSAGE: {
                xcb_client_message_event_t* cm =
                    (xcb_client_message_event_t*)event;

                if (cm->type == state->wm_protocols && cm->data.data32[0] == state->wm_delete_win) {
                    event_context ctx{};
                    event_fire(EVENT_CODE_APP_QUIT, NULL, ctx);
                    if(state->callbacks.on_quit) {
                        state->callbacks.on_quit(state->callbacks.user_data);
                    }
                    free(event);
                    return SN_FALSE;
                }
            } break;

            case XCB_KEY_PRESS: {
                xcb_key_press_event_t* kp = (xcb_key_press_event_t*)event;
                xkb_state_update_key(state->xkb_state, kp->detail, XKB_KEY_DOWN);
                xkb_keysym_t sym = xkb_state_key_get_one_sym(state->xkb_state, kp->detail);
                key_code key = xkb_keysym_to_key_code(sym);
                event_context ctx{};
                ctx.data.u16v[0] = (u16)key;
                event_fire(EVENT_CODE_KEY_PRESSED, NULL, ctx);
                if (state->callbacks.on_key)
                    state->callbacks.on_key(key, SN_TRUE, state->callbacks.user_data);
            } break;

            case XCB_KEY_RELEASE: {
                xcb_key_release_event_t* kr = (xcb_key_release_event_t*)event;
                xkb_keysym_t sym = xkb_state_key_get_one_sym(state->xkb_state, kr->detail);
                key_code key = xkb_keysym_to_key_code(sym);
                xkb_state_update_key(state->xkb_state, kr->detail, XKB_KEY_UP);
                event_context ctx{};
                ctx.data.u16v[0] = (u16)key;
                event_fire(EVENT_CODE_KEY_RELEASED, NULL, ctx);
                if (state->callbacks.on_key)
                    state->callbacks.on_key(key, SN_FALSE, state->callbacks.user_data);
            } break;

            case XCB_BUTTON_PRESS: {
                xcb_button_press_event_t* bp = (xcb_button_press_event_t*)event;
                if (bp->detail == 4 || bp->detail == 5) {
                    event_context ctx{};
                    ctx.data.i8v[0] = (bp->detail == 4) ? 1 : -1;
                    event_fire(EVENT_CODE_MOUSE_SCROLLED, NULL, ctx);
                    if (state->callbacks.on_scroll)
                        state->callbacks.on_scroll(ctx.data.i8v[0], state->callbacks.user_data);
                } else {
                    mouse_code btn = xcb_button_to_mouse_code(bp->detail);
                    event_context ctx{};
                    ctx.data.u16v[0] = (u16)btn;
                    event_fire(EVENT_CODE_BUTTON_PRESSED, NULL, ctx);
                    if (state->callbacks.on_button)
                        state->callbacks.on_button(btn, SN_TRUE, state->callbacks.user_data);
                }
            } break;

            case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t* br = (xcb_button_release_event_t*)event;
                if (br->detail == 4 || br->detail == 5) {
                    break;
                }
                mouse_code btn = xcb_button_to_mouse_code(br->detail);
                event_context ctx{};
                ctx.data.u16v[0] = (u16)btn;
                event_fire(EVENT_CODE_BUTTON_RELEASE, NULL, ctx);
                if (state->callbacks.on_button)
                    state->callbacks.on_button(btn, SN_FALSE, state->callbacks.user_data);
            } break;

            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t* mn = (xcb_motion_notify_event_t*)event;
                event_context ctx{};
                ctx.data.i16v[0] = (i16)mn->event_x;
                ctx.data.i16v[1] = (i16)mn->event_y;
                event_fire(EVENT_CODE_MOUSE_MOVE, NULL, ctx);
                if (state->callbacks.on_mouse)
                    state->callbacks.on_mouse(ctx.data.i16v[0], ctx.data.i16v[1],
                                              state->callbacks.user_data);
            } break;

            case XCB_CONFIGURE_NOTIFY: {
                xcb_configure_notify_event_t* cn = (xcb_configure_notify_event_t*)event;
                event_context ctx{};
                ctx.data.u16v[0] = cn->width;
                ctx.data.u16v[1] = cn->height;
                event_fire(EVENT_CODE_WINDOW_RESIZED, NULL, ctx);
                if (state->callbacks.on_resize)
                    state->callbacks.on_resize(ctx.data.u16v[0], ctx.data.u16v[1],
                                               state->callbacks.user_data);
            } break;
        }

        free(event);
    }

    return SN_TRUE;
}

void* platform_allocate(u64 size) {
    return malloc(size);
}

void* platform_allocate_aligned(u64 size, u64 alignment) {
    void* ptr = NULL;
    if(posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
}

void platform_free(void* block, b8 aligned) {
    (void)aligned;
    free(block);
}

void platform_zero_memory(void* block, u64 size) {
    memset(block, 0, size);
}

void platform_copy_memory(void* dest, const void* src, u64 size) {
    memcpy(dest, src, size);
}

void platform_move_memory(void* dest, const void* src, u64 size) {
    memmove(dest, src, size);
}

void platform_set_memory(void* dest, i32 value, u64 size) {
    memset(dest, value, size);
}

void platform_console_write(const char* msg, u8 color) {
    INFO("\033[%dm%s\033[0m", color, msg);
}

void platform_console_write_error(const char* msg, u8 color) {
    ERROR("\033[%dm%s\033[0m", color, msg);
}

f64 platform_get_absolute_time() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (f64)ts.tv_sec + (f64)ts.tv_nsec * 1e-9;
}

void platform_sleep(u64 ms) {
    struct timespec ts;
    ts.tv_sec = ms / 1000;
    ts.tv_nsec = (ms % 1000) * 1000000ULL;
    nanosleep(&ts, 0);
}

void platform_set_event_callbacks(platform_state* plat_state,
                                  const platform_event_callbacks* cb) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (cb) state->callbacks = *cb;
    else platform_zero_memory(&state->callbacks, sizeof(platform_event_callbacks));
}

#endif
