#pragma once

#include "defines.h"
#include "keyboard.h"
#include "mouse.h"

b8 input_init();
void input_shutdown();
void input_update(f64 delta_time);

// Keys
SNAPI b8 input_is_key_down(key_code kcode);
SNAPI b8 input_is_key_up(key_code kcode);
SNAPI b8 input_was_key_down(key_code kcode);
SNAPI b8 input_was_key_up(key_code kcode);
SNAPI const char* key_to_charkey(key_code kcode);

void input_process_key(key_code kcode, b8 pressed);

// Mouse
SNAPI b8 input_is_mouse_down(mouse_code mcode);
SNAPI b8 input_is_mouse_up(mouse_code mcode);
SNAPI b8 input_was_mouse_down(mouse_code mcode);
SNAPI b8 input_was_mouse_up(mouse_code mcode);
SNAPI void input_get_mouse_pos(i32* x, i32* y);
SNAPI void input_get_prev_mouse_pos(i32* x, i32* y);

void input_process_button(mouse_code mcode, b8 pressed);
void input_process_move(i16 x, i16 y);
void input_process_wheel(i8 z_delta);
