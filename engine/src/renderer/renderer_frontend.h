#pragma once

#include "renderer_types.inl"

struct static_mesh_data;
struct platform_state;

b8 renderer_init(const char* app_name, struct platform_state* plat_state);
void renderer_shutdown();
void renderer_resize(u16 width, u16 height);
b8 renderer_begin_frame(f32 delta_time);
b8 renderer_end_frame(f32 delta_time);
b8 renderer_draw_frame(render_packet* packet);
