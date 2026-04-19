#pragma once

#include "defines.h"
#include "platform/platform.h"

typedef struct event_context {
    // Solid 16-byte data
    union {
        i64 i64v[2];
        u64 u64v[2];
        f64 f64v[2];

        i32 i32v[4];
        u32 u32v[4];
        f32 f32v[4];

        i16 i16v[8];
        u16 u16v[8];

        i8 i8v[16];
        u8 u8v[16];

        char str[16];
    } data;
} event_context;

typedef enum event_code {
    // Applicaton control
    EVENT_CODE_APP_QUIT = 0x01,

    // Keyboard control
    EVENT_CODE_KEY_PRESSED = 0x02,
    EVENT_CODE_KEY_RELEASED = 0x03,

    // Mouse control
    EVENT_CODE_BUTTON_PRESSED = 0x04,
    EVENT_CODE_BUTTON_RELEASE = 0x05,
    EVENT_CODE_MOUSE_MOVE = 0x06,
    EVENT_CODE_MOUSE_SCROLLED = 0x07,

    // Window control
    EVENT_CODE_WINDOW_RESIZED = 0x08,

    EVENT_CODE_MAX_NUM

} event_code;

// Backward-compat aliases for older code paths.
#define EVENT_CODE_KEY_PRESS EVENT_CODE_KEY_PRESSED
#define EVENT_CODE_KEY_RELEASE EVENT_CODE_KEY_RELEASED
#define EVENT_CODE_BUTTON_PRESS EVENT_CODE_BUTTON_PRESSED
#define EVENT_CODE_MOUSE_WHEEL EVENT_CODE_MOUSE_SCROLLED
#define EVENT_CODE_MOUSE_MOVED EVENT_CODE_MOUSE_MOVE

typedef b8 (*sys_event)(u16 code, void* sender, void* listener_inst, const event_context* data);

SNAPI b8 event_init(void);
SNAPI b8 event_register(u16 code, void* listener, sys_event event);
SNAPI b8 event_unregister(u16 code, void* listener, sys_event event);
SNAPI b8 event_fire(u16 code, void* sender, event_context context);

SNAPI void print_event_msg(void);
