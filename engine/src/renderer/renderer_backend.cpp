#include "renderer_backend.h"
#include "core/logger.h"

b8 renderer_backend_create(renderer_backend_type type, struct platform_state* plat_state, renderer_backend* out_backend) {
    out_backend->plat_state = plat_state;

    switch(type) {
        case RENDERER_BACKEND_TYPE_VULKAN:
            // TODO: Vulkan backend api
            break;
        case RENDERER_BACKEND_TYPE_OPENGL:
            // TODO: OpenGL backend api
            break;
        case RENDERER_BACKEND_TYPE_DIRECTX:
            // TODO: DirectX backend api
            break;
        case RENDERER_BACKEND_TYPE_METAL:
            // TODO: Metal backend api
            break;
        default: 
            FATAL("Unsupported render API");
            return SN_FALSE; 
    }

    return SN_TRUE;

}

void renderer_destroy(renderer_backend* backend) {
    backend->init = 0;
    backend->shutdown = 0;
    backend->resized = 0;
    backend->begin_frame = 0;
    backend->end_frame = 0;
}


