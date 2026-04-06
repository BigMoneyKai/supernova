#pragma once

#include "defines.h"
#include "core/logger.h"

typedef struct darray {
    void* data;
    u64 length;
    u64 capacity;
    u64 stride;
} darray;

SNAPI log_process_type _darray_create(darray* arr, u64 capacity, u64 stride);
SNAPI log_process_type _darray_destroy(darray* arr);
SNAPI log_process_type _darray_resize(darray* arr);
SNAPI log_process_type _darray_push(darray* arr, const void* v_ptr);
SNAPI log_process_type _darray_insert(darray* arr, u64 idx, const void* v_ptr);
SNAPI log_process_type _darray_pop(darray* arr, void* dest);
SNAPI log_process_type _darray_pop_at(darray* arr, u64 idx, void* dest);

#define DARRAY_DEFAULT_CAPACITY 16
#define DARRAY_RESIZE_FACTOR 2

// External interface
#define darray_create(darr, type)\
    do {\
        log_process_type res = _darray_create((darr), DARRAY_DEFAULT_CAPACITY, sizeof(type));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data pointer to allocate");\
            } break;\
            case INVALID_CAPACITY: {\
                FATAL("invalid darray capacity");\
            } break;\
            case MALLOC_FAILED: {\
                FATAL("darray malloc failed");\
            } break;\
            case CORRECT: {\
                TRACE("darray created");\
            } break;\
        }\
    } while(0)

#define darray_destroy(darr)\
    do {\
        log_process_type res = _darray_destroy((darr));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data to destroy");\
            } break;\
            case CORRECT: {\
                TRACE("darray destroyed");\
            } break;\
        }\
    } while(0)

#define darray_resize(darr)\
    do {\
        log_process_type res = _darray_resize((darr));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data to resize");\
            } break;\
            case MALLOC_FAILED: {\
                FATAL("darray malloc failed");\
            } break;\
            case CORRECT: {\
                TRACE("darray resized");\
            } break;\
        }\
    } while(0)

#define darray_push(darr, val)\
    do {\
        typeof(val) temp = val;\
        log_process_type res = _darray_push((darr), (&temp));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data to push");\
            } break;\
            case MALLOC_FAILED: {\
                FATAL("darray malloc failed");\
            } break;\
            case CORRECT: {\
                TRACE("darray pushed '%s' back", #val);\
            } break;\
        }\
    } while(0)

#define darray_insert(darr, idx, val)\
    do {\
        typeof(val) temp = val;\
        log_process_type res = _darray_insert((darr), (idx), (&temp));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data to insert");\
            } break;\
            case OUT_OF_INDEX_BOUNDS: {\
                FATAL("index %lu out of bounds", idx);\
            } break;\
            case MALLOC_FAILED: {\
                FATAL("darray malloc failed");\
            } break;\
            case CORRECT: {\
                TRACE("darray inserted '%s' at %lu", #val, idx);\
            } break;\
        }\
    } while(0)

#define darray_pop(darr, dest)\
    do {\
        log_process_type res = _darray_pop((darr), (dest));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data to pop");\
            } break;\
            case CORRECT: {\
                TRACE("darray popped back to '%s'", #dest);\
            } break;\
        }\
    } while(0)

#define darray_pop_at(darr, idx, dest)\
    do {\
        log_process_type res = _darray_pop_at((darr), (idx), (dest));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid darray data to pop");\
            } break;\
            case OUT_OF_INDEX_BOUNDS: {\
                FATAL("index %lu out of bounds", idx);\
            } break;\
            case CORRECT: {\
                TRACE("darray popped at %lu to '%s'", idx, #dest);\
            } break;\
        }\
    } while(0)
