#include "platform.h"
#if SN_PLATFORM_WINDOWS

#include <stdlib.h>
#include <string.h>

#include <windows.h>
#include <windowsx.h>

#include "core/event.h"

typedef struct internal_state {
    HINSTANCE h_instance;
    HWND hwnd;
    b8 is_running;
    platform_event_callbacks callbacks;
} internal_state;

static internal_state* win32_state = NULL;

static key_code win32_vk_to_key_code(WPARAM w_param) {
    switch (w_param) {
        case 'A': return KEY_CODE_A;
        case 'B': return KEY_CODE_B;
        case 'C': return KEY_CODE_C;
        case 'D': return KEY_CODE_D;
        case 'E': return KEY_CODE_E;
        case 'F': return KEY_CODE_F;
        case 'G': return KEY_CODE_G;
        case 'H': return KEY_CODE_H;
        case 'I': return KEY_CODE_I;
        case 'J': return KEY_CODE_J;
        case 'K': return KEY_CODE_K;
        case 'L': return KEY_CODE_L;
        case 'M': return KEY_CODE_M;
        case 'N': return KEY_CODE_N;
        case 'O': return KEY_CODE_O;
        case 'P': return KEY_CODE_P;
        case 'Q': return KEY_CODE_Q;
        case 'R': return KEY_CODE_R;
        case 'S': return KEY_CODE_S;
        case 'T': return KEY_CODE_T;
        case 'U': return KEY_CODE_U;
        case 'V': return KEY_CODE_V;
        case 'W': return KEY_CODE_W;
        case 'X': return KEY_CODE_X;
        case 'Y': return KEY_CODE_Y;
        case 'Z': return KEY_CODE_Z;

        case '0': return KEY_CODE_0;
        case '1': return KEY_CODE_1;
        case '2': return KEY_CODE_2;
        case '3': return KEY_CODE_3;
        case '4': return KEY_CODE_4;
        case '5': return KEY_CODE_5;
        case '6': return KEY_CODE_6;
        case '7': return KEY_CODE_7;
        case '8': return KEY_CODE_8;
        case '9': return KEY_CODE_9;

        case VK_UP: return KEY_CODE_UP;
        case VK_DOWN: return KEY_CODE_DOWN;
        case VK_LEFT: return KEY_CODE_LEFT;
        case VK_RIGHT: return KEY_CODE_RIGHT;
        case VK_LSHIFT: return KEY_CODE_LSHIFT;
        case VK_RSHIFT: return KEY_CODE_RSHIFT;
        case VK_LCONTROL: return KEY_CODE_LCTRL;
        case VK_RCONTROL: return KEY_CODE_RCTRL;

        default: return 0;
    }
}

static mouse_code win32_button_to_mouse_code(u16 button) {
    switch(button) {
        case 1: return MOUSE_CODE_1;
        case 2: return MOUSE_CODE_2;
        case 3: return MOUSE_CODE_3;
        case 4: return MOUSE_CODE_4;
        default: return 0;
    }
}

static LRESULT CALLBACK win32_process_message(
    HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param) {

    event_context ctx = {0};

    switch (msg) {
        case WM_DESTROY:
        case WM_CLOSE:
            event_fire(EVENT_CODE_APP_QUIT, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_quit) {
                win32_state->callbacks.on_quit(win32_state->callbacks.user_data);
            }
            PostQuitMessage(0);
            return 0;

        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            ctx.data.u16v[0] = (u16)win32_vk_to_key_code(w_param);
            event_fire(EVENT_CODE_KEY_PRESSED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_key) {
                win32_state->callbacks.on_key((key_code)ctx.data.u16v[0], SN_TRUE,
                                              win32_state->callbacks.user_data);
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            ctx.data.u16v[0] = (u16)win32_vk_to_key_code(w_param);
            event_fire(EVENT_CODE_KEY_RELEASED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_key) {
                win32_state->callbacks.on_key((key_code)ctx.data.u16v[0], SN_FALSE,
                                              win32_state->callbacks.user_data);
            }
            break;

        case WM_LBUTTONDOWN: ctx.data.u16v[0] = MOUSE_CODE_1;
            event_fire(EVENT_CODE_BUTTON_PRESSED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_button) {
                win32_state->callbacks.on_button(win32_button_to_mouse_code(1), SN_TRUE,
                                                 win32_state->callbacks.user_data);
            }
            break;
        case WM_RBUTTONDOWN: ctx.data.u16v[0] = MOUSE_CODE_2;
            event_fire(EVENT_CODE_BUTTON_PRESSED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_button) {
                win32_state->callbacks.on_button(win32_button_to_mouse_code(2), SN_TRUE,
                                                 win32_state->callbacks.user_data);
            }
            break;
        case WM_MBUTTONDOWN: ctx.data.u16v[0] = MOUSE_CODE_3;
            event_fire(EVENT_CODE_BUTTON_PRESSED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_button) {
                win32_state->callbacks.on_button(win32_button_to_mouse_code(3), SN_TRUE,
                                                 win32_state->callbacks.user_data);
            }
            break;

        case WM_LBUTTONUP: ctx.data.u16v[0] = MOUSE_CODE_1;
            event_fire(EVENT_CODE_BUTTON_RELEASE, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_button) {
                win32_state->callbacks.on_button(win32_button_to_mouse_code(1), SN_FALSE,
                                                 win32_state->callbacks.user_data);
            }
            break;
        case WM_RBUTTONUP: ctx.data.u16v[0] = MOUSE_CODE_2;
            event_fire(EVENT_CODE_BUTTON_RELEASE, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_button) {
                win32_state->callbacks.on_button(win32_button_to_mouse_code(2), SN_FALSE,
                                                 win32_state->callbacks.user_data);
            }
            break;
        case WM_MBUTTONUP: ctx.data.u16v[0] = MOUSE_CODE_3;
            event_fire(EVENT_CODE_BUTTON_RELEASE, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_button) {
                win32_state->callbacks.on_button(win32_button_to_mouse_code(3), SN_FALSE,
                                                 win32_state->callbacks.user_data);
            }
            break;

        case WM_MOUSEMOVE:
            ctx.data.i16v[0] = (i16)GET_X_LPARAM(l_param);
            ctx.data.i16v[1] = (i16)GET_Y_LPARAM(l_param);
            event_fire(EVENT_CODE_MOUSE_MOVE, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_mouse) {
                win32_state->callbacks.on_mouse(ctx.data.i16v[0], ctx.data.i16v[1],
                                                win32_state->callbacks.user_data);
            }
            break;

        case WM_MOUSEWHEEL: {
            i8 delta = (GET_WHEEL_DELTA_WPARAM(w_param) > 0) ? 1 : -1;
            ctx.data.i8v[0] = delta;
            event_fire(EVENT_CODE_MOUSE_SCROLLED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_scroll) {
                win32_state->callbacks.on_scroll(delta, win32_state->callbacks.user_data);
            }
        } break;

        case WM_SIZE: {
            RECT r;
            GetClientRect(hwnd, &r);
            ctx.data.u16v[0] = (u16)(r.right  - r.left);
            ctx.data.u16v[1] = (u16)(r.bottom - r.top);
            event_fire(EVENT_CODE_WINDOW_RESIZED, NULL, ctx);
            if(win32_state && win32_state->callbacks.on_resize) {
                win32_state->callbacks.on_resize(ctx.data.u16v[0], ctx.data.u16v[1],
                                                 win32_state->callbacks.user_data);
            }
        } break;

        default:
            return DefWindowProcA(hwnd, msg, w_param, l_param);
    }
    return 0;
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
    internal_state* state = (internal_state*)plat_state->internal_state;
    platform_zero_memory(state, sizeof(internal_state));

    state->h_instance = GetModuleHandle(0);

    WNDCLASSA wc = {0};
    wc.style = CS_DBLCLKS;
    wc.lpfnWndProc = win32_process_message;
    wc.hInstance = state->h_instance;
    wc.lpszClassName = "engine_window_class";

    if(!RegisterClassA(&wc)) {
        return SN_FALSE;
    }

    HWND hwnd = CreateWindowExA(
        0,
        wc.lpszClassName,
        application_name,
        WS_OVERLAPPEDWINDOW,
        x, y, width, height,
        0, 0,
        state->h_instance,
        0
    );

    if(!hwnd) return SN_FALSE;

    state->hwnd = hwnd;
    state->is_running = SN_TRUE;
    win32_state = state;

    ShowWindow(hwnd, SW_SHOW);
    return SN_TRUE;
}

void platform_shutdown(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if(state->hwnd) {
        DestroyWindow(state->hwnd);
    }
    free(state);
    plat_state->internal_state = NULL;
    win32_state = NULL;
}

b8 platform_pump_messages(platform_state* plat_state) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    MSG msg;
    while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
        if(msg.message == WM_QUIT) {
            state->is_running = SN_FALSE;
            return SN_FALSE;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return SN_TRUE;
}

void* platform_allocate(u64 size) {
    return malloc(size);
}

void* platform_allocate_aligned(u64 size, u64 alignment) {
    return _aligned_malloc((size_t)size, (size_t)alignment);
}

void platform_free(void* block, b8 aligned) {
    if(aligned) {
        _aligned_free(block);
    } else {
        free(block);
    }
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
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, color);
    OutputDebugStringA(msg);
    INFO("%s", msg);
}

void platform_console_write_error(const char* msg, u8 color) {
    HANDLE console = GetStdHandle(STD_ERROR_HANDLE);
    SetConsoleTextAttribute(console, color);
    OutputDebugStringA(msg);
    ERROR("\033[%sm%s\033[0m", color, msg);
}

f64 platform_get_absolute_time() {
    static LARGE_INTEGER freq;
    static b8 initialized = SN_FALSE;

    if(!initialized) {
        QueryPerformanceFrequency(&freq);
        initialized = SN_TRUE;
    }

    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);

    return (f64)now.QuadPart / (f64)freq.QuadPart;
}

void platform_sleep(u64 ms) {
    if(ms > 0xFFFFFFFF) {
        ms = 0xFFFFFFFF; // sleep clamp
    }
    Sleep((DWORD)ms);
}

void platform_set_event_callbacks(platform_state* plat_state,
                                  const platform_event_callbacks* cb) {
    internal_state* state = (internal_state*)plat_state->internal_state;
    if (cb) state->callbacks = *cb;
    else platform_zero_memory(&state->callbacks, sizeof(platform_event_callbacks));
}

#endif
