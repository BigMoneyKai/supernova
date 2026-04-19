#pragma once

#include "defines.h"

typedef struct app_config {
    i16 start_pos_x;
    i16 start_pos_y;
    i16 start_width;
    i16 start_height;

    const char* name;
} app_config;

SNAPI b8 app_create(app_config* config);
SNAPI b8 app_run(void);
