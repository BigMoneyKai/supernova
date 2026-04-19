#pragma once

#include "defines.h"
#include "core/keyboard.h"
#include "core/mouse.h"

#define ALIGNMENT 16

typedef enum sn_button {
    BUTTON_UNKNOWN = 0,
    BUTTON_LEFT,
    BUTTON_RIGHT,
    BUTTON_MIDDLE,
    BUTTON_SCROLL_UP,
    BUTTON_SCROLL_DOWN,
    BUTTON_MAX
} sn_button;

typedef struct platform_event_callbacks {
    void (*on_quit)    (void* user);
    void (*on_key)     (key_code key,    b8 pressed,  void* user);
    void (*on_button)  (mouse_code btn,  b8 pressed,  void* user);
    void (*on_mouse)   (i16 x, i16 y,                 void* user);
    void (*on_scroll)  (i8 delta,                     void* user);
    void (*on_resize)  (u16 w, u16 h,                 void* user);
    void* user_data;
} platform_event_callbacks;

typedef struct platform_state {
    void* internal_state;
} platform_state;

b8 platform_startup(
    platform_state* plat_state,
    const char* application_name,
    i32 x,
    i32 y,
    i32 width,
    i32 height
);
void platform_shutdown(platform_state* plat_state);
b8 platform_pump_messages(platform_state* plat_state);

void* platform_allocate(u64 size);
void* platform_allocate_aligned(u64 size, u64 alignment);
void platform_free(void* block, b8 aligned);
void platform_zero_memory(void* block, u64 size);
void platform_copy_memory(void* dest, const void* src, u64 size);
void platform_move_memory(void* dest, const void* src, u64 size);
void platform_set_memory(void* dest, i32 value, u64 size);

void platform_console_write(const char* msg, u8 color);
void platform_console_write_error(const char* msg, u8 color);

f64 platform_get_absolute_time(void);
void platform_sleep(u64 ms);

void platform_set_event_callbacks(platform_state* plat_state, const platform_event_callbacks* callbacks);
