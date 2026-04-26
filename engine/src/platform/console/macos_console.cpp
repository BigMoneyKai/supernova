#include "platform/platform.h"

#if SN_PLATFORM_APPLE

#include "core/logger.h"

void platform_console_write(const char* msg, u8 color) {
    INFO("\033[%dm%s\033[0m", color, msg);
}

void platform_console_write_error(const char* msg, u8 color) {
    ERROR("\033[%dm%s\033[0m", color, msg);
}

#endif
