#include "renderer_frontend.h"
#include "renderer_backend.h"
#include "core/memory.h"
#include "core/logger.h"

static renderer_backend* backend = 0;

b8 renderer_init(const char* app_name, struct platform_state* plat_state) {
    backend = reinterpret_cast<renderer_backend*>(snmalloc(sizeof(renderer_backend), MEM_TAG_RENDERER));

    renderer_backend_create(RENDERER_BACKEND_TYPE_VULKAN, plat_state, backend);
    backend->frame_num = 0;
    
    if(!backend->init(backend, app_name, plat_state)) {
        FATAL("backend initialization failure");
        return SN_FALSE;
    }   
    
    return SN_TRUE;
}

void renderer_shutdown() {

    backend->shutdown(backend);
    snmfree(backend, sizeof(renderer_backend), MEM_TAG_RENDERER);
}

b8 renderer_begin_frame(f32 delta_time) {
    return backend->begin_frame(backend, delta_time);
}

b8 renderer_end_frame(f32 delta_time) {
    b8 res = backend->end_frame(backend, delta_time);
    backend->frame_num++;
    return res;
}

b8 renderer_draw_frame(render_packet* packet) {
    if(!renderer_begin_frame(packet->delta_time)) {
        FATAL("renderer begin frame failure, app shutting down...");
        return SN_FALSE;
    }
    if(!renderer_end_frame(packet->delta_time)) {
        FATAL("renderer end frame failure, app shutting down...");
        return SN_FALSE;
    }
    return SN_TRUE;

}
