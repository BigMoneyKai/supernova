#include "platform/platform.h"
#include "core/logger.h"

#if SN_PLATFORM_LINUX

void platform_console_write(const char* msg, u8 color) {
    INFO("\033[%dm%s\033[0m", color, msg);
}

void platform_console_write_error(const char* msg, u8 color) {
    ERROR("\033[%dm%s\033[0m", color, msg);
}

#endif
