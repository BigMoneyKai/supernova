#pragma once

#include "renderer_types.inl"

struct platform_state;

b8 renderer_backend_create(renderer_backend_type type, struct platform_state* plat_state, renderer_backend* out_backend);
void renderer_destroy(renderer_backend* backend);




