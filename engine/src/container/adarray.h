// allocation-determined array (adarray)

#pragma once

#include "defines.h"
#include "core/logger.h"

typedef struct adarray {
    void* data;
    u64 length;
    u64 capacity;
    u64 stride;
} adarray;

SNAPI log_process_type _adarray_create(adarray* arr, u64 capacity, u64 stride);
SNAPI log_process_type _adarray_destroy(adarray* arr);
SNAPI log_process_type _adarray_resize(adarray* arr);
SNAPI log_process_type _adarray_push(adarray* arr, const void* v_ptr);
SNAPI log_process_type _adarray_insert(adarray* arr, u64 idx, const void* v_ptr);
SNAPI log_process_type _adarray_pop(adarray* arr, void* dest);
SNAPI log_process_type _adarray_pop_at(adarray* arr, u64 idx, void* dest);

#define ADARRAY_DEFAULT_CAPACITY 16
#define ADARRAY_RESIZE_FACTOR 2

// External interface
#define adarray_create(darr, type)\
    do {\
        log_process_type res = _adarray_create((darr), ADARRAY_DEFAULT_CAPACITY, sizeof(type));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to allocate");\
                break;\
            case INVALID_CAPACITY:\
                FATAL("invalid adarray capacity");\
                break;\
            case MALLOC_FAILED:\
                FATAL("adarray malloc failure");\
                break;\
            case CORRECT:\
                TRACE("adarray created");\
                break;\
        }\
    } while(0)

#define adarray_destroy(darr)\
    do {\
        log_process_type res = _adarray_destroy((darr));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to destroy");\
                break;\
            case INVALID_CAPACITY:\
                FATAL("invalid adarray capacity");\
                break;\
            case MALLOC_FAILED:\
                FATAL("adarray malloc failure");\
                break;\
            case CORRECT:\
                TRACE("adarray destroyed");\
                break;\
        }\
    } while(0)

#define adarray_resize(darr)\
    do {\
        log_process_type res = _adarray_resize((darr));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to resize");\
                break;\
            case MALLOC_FAILED:\
                FATAL("adarray malloc failure");\
                break;\
            case CORRECT:\
                TRACE("adarray resized");\
                break;\
        }\
    } while(0)

#define adarray_push(darr, val)\
    do {\
        auto temp = val;\
        log_process_type res = _adarray_push((darr), (&temp));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to push element");\
                break;\
            case OUT_OF_MEMORY:\
                FATAL("adarray is full");\
                break;\
            case CORRECT:\
                TRACE("adarray pushed '%s' back", #val);\
                break;\
        }\
    } while(0)

#define adarray_insert(darr, idx, val)\
    do {\
        auto temp = val;\
        log_process_type res = _adarray_insert((darr), (idx), (&temp));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to insert element");\
                break;\
            case OUT_OF_MEMORY:\
                FATAL("adarray is full");\
                break;\
            case OUT_OF_INDEX_BOUNDS:\
                FATAL("index %lu out of adarray bounds", idx);\
                break;\
            case CORRECT:\
                TRACE("adarray inserted '%s' at %lu", #val, idx);\
                break;\
        }\
    } while(0)

#define adarray_pop(darr, dest)\
    do {\
        log_process_type res = _adarray_pop((darr), (dest));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to pop element");\
                break;\
            case CORRECT:\
                TRACE("adarray popped back to '%s'", #dest);\
                break;\
        }\
    } while(0)

#define adarray_pop_at(darr, idx, dest)\
    do {\
        log_process_type res = _adarray_pop_at((darr), (idx), (dest));\
        switch(res) {\
            case NULL_PTR:\
                FATAL("invalid adarray data pointer to pop element");\
                break;\
            case OUT_OF_INDEX_BOUNDS:\
                FATAL("index %lu out of adarray bounds", idx);\
                break;\
            case CORRECT:\
                TRACE("adarray popped at %lu to '%s'", idx, #dest);\
                break;\
        }\
    } while(0)
