#include "platform/platform.h"

#if SN_PLATFORM_WINDOWS

#include <windows.h>

#include "core/logger.h"

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
    ERROR("%s", msg);
}

#endif
