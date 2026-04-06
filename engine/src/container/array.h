#pragma once

#include "defines.h"
#include "core/logger.h"

typedef struct array {
    void* data;
    u64 length;
    u64 capacity;
    u64 stride;
} array;

SNAPI log_process_type _array_create(array* arr, u64 capacity, u64 stride);
SNAPI log_process_type _array_destroy(array* arr);
SNAPI log_process_type _array_push(array* arr, const void* v_ptr);
SNAPI log_process_type _array_insert(array* arr, u64 idx, const void* v_ptr);
SNAPI log_process_type _array_pop(array* arr, void* dest);
SNAPI log_process_type _array_pop_at(array* arr, u64 idx, void* dest);

#define ARRAY_DEFAULT_CAPACITY 16

// External interface
#define array_create(darr, type)\
    do {\
        log_process_type res = _array_create((darr), ARRAY_DEFAULT_CAPACITY, sizeof(type));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid array data pointer to allocate");\
            } break;\
            case INVALID_CAPACITY: {\
                FATAL("invalid array capacity");\
            } break;\
            case MALLOC_FAILED: {\
                FATAL("array malloc failure");\
            } break;\
            case CORRECT: {\
                TRACE("array created");\
            } break;\
        }\
    } while(0)

#define array_destroy(darr)\
    do {\
        log_process_type res = _array_destroy((darr));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid array data to destroy");\
            } break;\
            case CORRECT: {\
                TRACE("array destroyed");\
            } break;\
        }\
    } while(0)

#define array_push(darr, val)\
    do {\
        typeof(val) temp = val;\
        log_process_type res = _array_push((darr), (&temp));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid array data to push");\
            } break;\
            case OUT_OF_MEMORY: {\
                FATAL("array is full");\
            } break;\
            case CORRECT: {\
                TRACE("array pushed '%s' back", #val);\
            } break;\
        }\
    } while(0)

#define array_insert(darr, idx, val)\
    do {\
        typeof(val) temp = val;\
        log_process_type res = _array_insert((darr), (idx), (&temp));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid array data to insert");\
            } break;\
            case OUT_OF_INDEX_BOUNDS: {\
                FATAL("index %lu out of bounds", idx);\
            } break;\
            case OUT_OF_MEMORY: {\
                FATAL("array is full");\
            } break;\
            case CORRECT: {\
                TRACE("array inserted '%s' at %lu", #val, idx);\
            } break;\
        }\
    } while(0)

#define array_pop(darr, dest)\
    do {\
        log_process_type res = _array_pop((darr), (dest));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid array data to pop");\
            } break;\
            case MEMORY_EMPTY: {\
                FATAL("array is empty");\
            } break;\
            case CORRECT: {\
                TRACE("array popped back to '%s'", #dest);\
            } break;\
        }\
    } while(0)

#define array_pop_at(darr, idx, dest)\
    do {\
        log_process_type res = _array_pop_at((darr), (idx), (dest));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid array data to pop");\
            } break;\
            case MEMORY_EMPTY: {\
                FATAL("array is empty");\
            } break;\
            case OUT_OF_INDEX_BOUNDS: {\
                FATAL("index %lu out of bounds", idx);\
            } break;\
            case CORRECT: {\
                TRACE("array popped at %lu to '%s'", idx, #dest);\
            } break;\
        }\
    } while(0)
