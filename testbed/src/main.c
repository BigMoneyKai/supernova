#include <core/logger.h>
#include <core/app.h>
#include <core/event.h>
#include <core/input.h>
#include <core/memory.h>

#include <stdlib.h>

static b8 on_key_pressed(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    key_code key = (key_code)data->data.u16v[0];
    INFO("key down: %u (is_down=%u)", key, input_is_key_down(key));
    return SN_FALSE;
}

static b8 on_key_released(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    key_code key = (key_code)data->data.u16v[0];
    INFO("key up: %u (is_down=%u)", key, input_is_key_down(key));
    return SN_FALSE;
}

static b8 on_button_pressed(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    mouse_code btn = (mouse_code)data->data.u16v[0];
    INFO("mouse down: %u", btn);
    return SN_FALSE;
}

static b8 on_button_released(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    mouse_code btn = (mouse_code)data->data.u16v[0];
    INFO("mouse up: %u", btn);
    return SN_FALSE;
}

static b8 on_mouse_move(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    INFO("mouse move: %d, %d", data->data.i16v[0], data->data.i16v[1]);
    return SN_FALSE;
}

static b8 on_mouse_scroll(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    INFO("mouse scroll: %d", data->data.i8v[0]);
    return SN_FALSE;
}

static b8 on_window_resized(u16 code, void* sender, void* listener, const event_context* data) {
    (void)code; (void)sender; (void)listener;
    INFO("window resized: %u x %u", data->data.u16v[0], data->data.u16v[1]);
    return SN_FALSE;
}

int main() {
    app_config config = {
        0, 0,
        1920, 1080,
        "supernova"
    };

    memsys_state state;
    snminit(&state);

    // memory macro tests
    {
        void* block = snmalloc(64, MEM_TAG_ENGINE);
        snmset(block, 0xAB, 64);
        snmzero(block, 64);

        void* block2 = snmalloc(64, MEM_TAG_ENGINE);
        snmcopy(block2, block, 64);
        snmmove(block, block2, 64);

        u64 str_mark = snm_string_mark();
        void* s1 = snmalloc(128, MEM_TAG_STRING);
        (void)s1;
        snm_string_reset_to_mark(str_mark);

        u64 job_mark = snm_job_mark();
        void* j1 = snmalloc(128, MEM_TAG_JOB);
        (void)j1;
        snm_job_reset_to_mark(job_mark);

        snmfree(block, 64, MEM_TAG_ENGINE);
        snmfree(block2, 64, MEM_TAG_ENGINE);
    }

    update_mstats_tui();

    INFO("app creating...");
    if(app_create(&config)) {
        INFO("app creating successfully");
    } else {
        FATAL("app failed to create");
        return EXIT_FAILURE;
    }

    event_register(EVENT_CODE_KEY_PRESSED, NULL, on_key_pressed);
    event_register(EVENT_CODE_KEY_RELEASED, NULL, on_key_released);
    event_register(EVENT_CODE_BUTTON_PRESSED, NULL, on_button_pressed);
    event_register(EVENT_CODE_BUTTON_RELEASE, NULL, on_button_released);
    event_register(EVENT_CODE_MOUSE_MOVE, NULL, on_mouse_move);
    event_register(EVENT_CODE_MOUSE_SCROLLED, NULL, on_mouse_scroll);
    event_register(EVENT_CODE_WINDOW_RESIZED, NULL, on_window_resized);

    INFO("app runtime");
    if(app_run()) {
        INFO("app runtime exited successfully");
    } else {
        FATAL("app runtime crashed");
        return EXIT_FAILURE;
    }
    return 0;
}
