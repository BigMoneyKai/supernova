#include "input.h"
#include "event.h"
#include "memory.h"
#include "logger.h"

typedef struct keyboard_state {
    b8 keys[KEY_CODE_MAX_NUM];
} keyboard_state;

typedef struct mouse_state {
    i16 pos_x;
    i16 pos_y;
    i8 wheel;
    b8 buttons[MOUSE_CODE_MAX_NUM];
} mouse_state;

typedef struct input_state {
    keyboard_state keyboard_curr;
    keyboard_state keyboard_prev;

    mouse_state mouse_curr;
    mouse_state mouse_prev;
} input_state;

static b8 is_inited = SN_FALSE;
static input_state input = {0};

static b8 input_on_event(u16 code, void* sender, void* listener, const event_context* data) {
    (void)sender;
    (void)listener;

    if(!data) {
        return SN_FALSE;
    }

    switch(code) {
        case EVENT_CODE_KEY_PRESSED:
        case EVENT_CODE_KEY_RELEASED: {
            key_code key = (key_code)data->data.u16v[0];
            b8 pressed = (code == EVENT_CODE_KEY_PRESSED) ? SN_TRUE : SN_FALSE;
            input_process_key(key, pressed);
        } break;

        case EVENT_CODE_BUTTON_PRESSED:
        case EVENT_CODE_BUTTON_RELEASE: {
            mouse_code btn = (mouse_code)data->data.u16v[0];
            b8 pressed = (code == EVENT_CODE_BUTTON_PRESSED) ? SN_TRUE : SN_FALSE;
            input_process_button(btn, pressed);
        } break;

        case EVENT_CODE_MOUSE_MOVE: {
            i16 x = data->data.i16v[0];
            i16 y = data->data.i16v[1];
            input_process_move(x, y);
        } break;

        case EVENT_CODE_MOUSE_SCROLLED: {
            i8 delta = data->data.i8v[0];
            input_process_wheel(delta);
        } break;

        default:
            break;
    }

    return SN_FALSE;
}

b8 input_init() {
    if(is_inited) {
        ERROR("input system already initialized");
        return SN_FALSE;
    }
    snmzero(&input, sizeof(input));
    is_inited = SN_TRUE;

    event_register(EVENT_CODE_KEY_PRESSED, &input, input_on_event);
    event_register(EVENT_CODE_KEY_RELEASED, &input, input_on_event);
    event_register(EVENT_CODE_BUTTON_PRESSED, &input, input_on_event);
    event_register(EVENT_CODE_BUTTON_RELEASE, &input, input_on_event);
    event_register(EVENT_CODE_MOUSE_MOVE, &input, input_on_event);
    event_register(EVENT_CODE_MOUSE_SCROLLED, &input, input_on_event);

    return SN_TRUE;
}

void input_shutdown() {
    if(!is_inited) {
        return;
    }

    event_unregister(EVENT_CODE_KEY_PRESSED, &input, input_on_event);
    event_unregister(EVENT_CODE_KEY_RELEASED, &input, input_on_event);
    event_unregister(EVENT_CODE_BUTTON_PRESSED, &input, input_on_event);
    event_unregister(EVENT_CODE_BUTTON_RELEASE, &input, input_on_event);
    event_unregister(EVENT_CODE_MOUSE_MOVE, &input, input_on_event);
    event_unregister(EVENT_CODE_MOUSE_SCROLLED, &input, input_on_event);

    snmzero(&input, sizeof(input));
    is_inited = SN_FALSE;
}

void input_update(f64 delta_time) {
    if(!is_inited) {
        return;
    }
    (void)delta_time;
    input.keyboard_prev = input.keyboard_curr;
    input.mouse_prev = input.mouse_curr;

    input.mouse_curr.wheel = 0;
}

// Keys
b8 input_is_key_down(key_code kcode) {
    if(!is_inited || kcode >= KEY_CODE_MAX_NUM) {
        return SN_FALSE;
    }
    return input.keyboard_curr.keys[kcode];
}

b8 input_is_key_up(key_code kcode) {
    if(!is_inited || kcode >= KEY_CODE_MAX_NUM) {
        return SN_TRUE;
    }
    return !input.keyboard_curr.keys[kcode];
}

b8 input_was_key_down(key_code kcode) {
    if(!is_inited || kcode >= KEY_CODE_MAX_NUM) {
        return SN_FALSE;
    }
    return input.keyboard_prev.keys[kcode];
}

b8 input_was_key_up(key_code kcode) {
    if(!is_inited || kcode >= KEY_CODE_MAX_NUM) {
        return SN_TRUE;
    }
    return !input.keyboard_prev.keys[kcode];
}

const char* key_to_charkey(key_code kcode) {
    switch(kcode) {
        case KEY_CODE_A: return "A";
        case KEY_CODE_B: return "B";
        case KEY_CODE_C: return "C";
        case KEY_CODE_D: return "D";
        case KEY_CODE_E: return "E";
        case KEY_CODE_F: return "F";
        case KEY_CODE_G: return "G";
        case KEY_CODE_H: return "H";
        case KEY_CODE_I: return "I";
        case KEY_CODE_J: return "J";
        case KEY_CODE_K: return "K";
        case KEY_CODE_L: return "L";
        case KEY_CODE_M: return "M";
        case KEY_CODE_N: return "N";
        case KEY_CODE_O: return "O";
        case KEY_CODE_P: return "P";
        case KEY_CODE_Q: return "Q";
        case KEY_CODE_R: return "R";
        case KEY_CODE_S: return "S";
        case KEY_CODE_T: return "T";
        case KEY_CODE_U: return "U";
        case KEY_CODE_V: return "V";
        case KEY_CODE_W: return "W";
        case KEY_CODE_X: return "X";
        case KEY_CODE_Y: return "Y";
        case KEY_CODE_Z: return "Z";

        case KEY_CODE_a: return "a";
        case KEY_CODE_b: return "b";
        case KEY_CODE_c: return "c";
        case KEY_CODE_d: return "d";
        case KEY_CODE_e: return "e";
        case KEY_CODE_f: return "f";
        case KEY_CODE_g: return "g";
        case KEY_CODE_h: return "h";
        case KEY_CODE_i: return "i";
        case KEY_CODE_j: return "j";
        case KEY_CODE_k: return "k";
        case KEY_CODE_l: return "l";
        case KEY_CODE_m: return "m";
        case KEY_CODE_n: return "n";
        case KEY_CODE_o: return "o";
        case KEY_CODE_p: return "p";
        case KEY_CODE_q: return "q";
        case KEY_CODE_r: return "r";
        case KEY_CODE_s: return "s";
        case KEY_CODE_t: return "t";
        case KEY_CODE_u: return "u";
        case KEY_CODE_v: return "v";
        case KEY_CODE_w: return "w";
        case KEY_CODE_x: return "x";
        case KEY_CODE_y: return "y";
        case KEY_CODE_z: return "z";

        case KEY_CODE_0: return "0";
        case KEY_CODE_1: return "1";
        case KEY_CODE_2: return "2";
        case KEY_CODE_3: return "3";
        case KEY_CODE_4: return "4";
        case KEY_CODE_5: return "5";
        case KEY_CODE_6: return "6";
        case KEY_CODE_7: return "7";
        case KEY_CODE_8: return "8";
        case KEY_CODE_9: return "9";
        default: return "?";
    }
}

void input_process_key(key_code kcode, b8 pressed) {
    if(!is_inited || kcode == 0 || kcode >= KEY_CODE_MAX_NUM) {
        return;
    }
    input.keyboard_curr.keys[kcode] = pressed;
}

// Mouse
b8 input_is_mouse_down(mouse_code mcode) {
    if(!is_inited || mcode >= MOUSE_CODE_MAX_NUM) {
        return SN_FALSE;
    }
    return input.mouse_curr.buttons[mcode];
}

b8 input_is_mouse_up(mouse_code mcode) {
    if(!is_inited || mcode >= MOUSE_CODE_MAX_NUM) {
        return SN_TRUE;
    }
    return !input.mouse_curr.buttons[mcode];
}

b8 input_was_mouse_down(mouse_code mcode) {
    if(!is_inited || mcode >= MOUSE_CODE_MAX_NUM) {
        return SN_FALSE;
    }
    return input.mouse_prev.buttons[mcode];
}

b8 input_was_mouse_up(mouse_code mcode) {
    if(!is_inited || mcode >= MOUSE_CODE_MAX_NUM) {
        return SN_TRUE;
    }
    return !input.mouse_prev.buttons[mcode];
}

void input_get_mouse_pos(i32* x, i32* y) {
    if(!is_inited) {
        return;
    }
    if(x) {
        *x = input.mouse_curr.pos_x;
    }
    if(y) {
        *y = input.mouse_curr.pos_y;
    }
}

void input_get_prev_mouse_pos(i32* x, i32* y) {
    if(!is_inited) {
        return;
    }
    if(x) {
        *x = input.mouse_prev.pos_x;
    }
    if(y) {
        *y = input.mouse_prev.pos_y;
    }
}

void input_process_button(mouse_code mcode, b8 pressed) {
    if(!is_inited || mcode == 0 || mcode >= MOUSE_CODE_MAX_NUM) {
        return;
    }
    input.mouse_curr.buttons[mcode] = pressed;
}

void input_process_move(i16 x, i16 y) {
    if(!is_inited) {
        return;
    }
    input.mouse_curr.pos_x = x;
    input.mouse_curr.pos_y = y;
}

void input_process_wheel(i8 z_delta) {
    if(!is_inited) {
        return;
    }
    input.mouse_curr.wheel += z_delta;
}
