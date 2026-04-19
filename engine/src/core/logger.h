#pragma once

#include "defines.h"

#define LOG_WARN_ENABLED    1
#define LOG_INFO_ENABLED    1
#define LOG_DEBUG_ENABLED   1
#define LOG_TRACE_ENABLED   1

#if SNRELEASE == 1
#define LOG_DEBUG_ENABLED 0
#define LOG_TRACE_ENABLED 0
#endif

#if SN_PLATFORM_WINDOWS == 1
#define RESET       ""
#define GREEN       ""
#define RED         ""
#define DARK_RED    ""
#define LIGHT_BLUE  ""
#define MAGENTA     ""
#else
#define RESET       "\033[0m"
#define GREEN       "\033[32m"
#define RED         "\033[31m"
#define DARK_RED    "\033[1;31m"
#define LIGHT_BLUE  "\033[94m"
#define MAGENTA     "\033[35m"
#endif

typedef enum log_level {
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN  = 2,
    LOG_LEVEL_INFO  = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5,
} log_level;

typedef enum log_process_type {
    CORRECT = 0,

    // container
    OUT_OF_MEMORY,
    MEMORY_EMPTY,
    INVALID_CAPACITY,
    MALLOC_FAILED,
    NULL_PTR,
    RESIZE_FAILED,
    OUT_OF_INDEX_BOUNDS,
    INVALID_INPUT,

    // memory
    MEMSYS_NULL,
    DOUBLE_FREE,
    OUT_OF_MEM_BOUNDS,
    USE_AFTER_FREE,
    MEM_LEAK,
    DANGLING_PTR,
    MISALIGNMENT,
    UB,
    STACK_OVERFLOW,

    // general
    NON_INIT,
    DOUBLE_INIT,

    EXCEPT_MAX_NUM
} log_process_type;

b8 init_logging(void);
void quit_logging(void);
void log_output(log_level level, const char* file_path, i32 line, const char* msg, ...);

// fatal
#define FATAL(msg, ...)\
    do {\
        log_output(LOG_LEVEL_FATAL, __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
    } while(0)

// error
#define ERROR(msg, ...)\
    do {\
        log_output(LOG_LEVEL_ERROR, __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
    } while(0)

// warning
#if LOG_WARN_ENABLED == 1
#define WARN(msg, ...)\
    do {\
        log_output(LOG_LEVEL_WARN, __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
    } while(0)
#else
#define WARN(msg, ...) (void(0))
#endif

// info
#if LOG_INFO_ENABLED == 1
#define INFO(msg, ...)\
    do {\
        log_output(LOG_LEVEL_INFO, __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
    } while(0)
#else
#define INFO(msg, ...) (void(0))
#endif

// debug
#if LOG_DEBUG_ENABLED == 1
#define DEBUG(msg, ...)\
    do {\
        log_output(LOG_LEVEL_DEBUG, __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
    } while(0)
#else
#define DEBUG(msg, ...) (void(0))
#endif

// trace
#if LOG_TRACE_ENABLED == 1
#define TRACE(msg, ...)\
    do {\
        log_output(LOG_LEVEL_TRACE, __FILE__, __LINE__, msg __VA_OPT__(,) __VA_ARGS__);\
    } while(0)
#else
#define TRACE(msg, ...) (void(0))
#endif
