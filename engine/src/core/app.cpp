#include "app.h"
#include "platform/platform.h"
#include "core/logger.h"
#include "core/memory.h"
#include "core/event.h"
#include "core/input.h"
#include "core/clock.h"

typedef struct app_state {
    b8 is_running;
    b8 is_suspended;
    platform_state platform;
    i16 width;
    i16 height;
    clock clock;
    f64 last_time;

} app_state;

static b8 is_inited = SN_FALSE;
static app_state app;

b8 app_create(app_config* config) {
    if(is_inited) {
        ERROR("'app_create' invoked twice");
        return SN_FALSE;
    }
    init_logging();

    app.is_running = SN_TRUE;
    app.is_suspended = SN_FALSE;

    if(!event_init()) {
        ERROR("event system init failed");
        return SN_FALSE;
    }

    if(!input_init()) {
        ERROR("input system init failed");
        return SN_FALSE;
    }

    if(!platform_startup(
        &app.platform,
        config->name,
        config->start_pos_x, config->start_pos_y,
        config->start_width, config->start_height)) {
        ERROR("platform startup failed");
        return SN_FALSE;
    }
    app.last_time = platform_get_absolute_time();
    is_inited = SN_TRUE;
    return SN_TRUE;
}

b8 app_run(void) {
    clock_start(&app.clock);
    clock_update(&app.clock);
    app.last_time = app.clock.elapsed;

    while(app.is_running) {
        f64 current = platform_get_absolute_time();
        f64 delta = current - app.last_time;
        app.last_time = current;

        if(!platform_pump_messages(&app.platform)) {
            app.is_running = SN_FALSE;
        }

        input_update(delta);
        print_event_msg();
    }

    app.is_running = SN_FALSE;

    clock_shutdown(&app.clock);
    platform_shutdown(&app.platform);
    input_shutdown();
    return SN_TRUE;
}
