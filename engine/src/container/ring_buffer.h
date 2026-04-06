#pragma once

#include "defines.h"
#include "core/logger.h"

typedef struct ring_buffer {
    void* data;
    u64 head;
    u64 tail;
    u64 length;
    u64 capacity;
    u64 stride;
} ring_buffer;

SNAPI log_process_type _ring_buffer_create(ring_buffer* rb, u64 capacity, u64 stride);
SNAPI log_process_type _ring_buffer_destroy(ring_buffer* rb);
SNAPI log_process_type _ring_buffer_resize(ring_buffer* rb);
SNAPI log_process_type _ring_buffer_push(ring_buffer* rb, const void* v_ptr);
SNAPI log_process_type _ring_buffer_pop(ring_buffer* rb, void* dest);

#define RING_BUFFER_DEFAULT_CAPACITY 16
#define RING_BUFFER_RESIZE_FACTOR 2

// External interface
#define ring_buffer_create(rb, type)\
    do {\
        log_process_type res = _ring_buffer_create((rb), RING_BUFFER_DEFAULT_CAPACITY, sizeof(type));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid ring buffer data pointer to allocate");\
            } break;\
            case INVALID_CAPACITY: {\
                FATAL("invalid ring buffer capacity");\
            } break;\
            case MALLOC_FAILED: {\
                FATAL("ring buffer malloc failure");\
            } break;\
            case CORRECT: {\
                TRACE("ring buffer created successfully");\
            } break;\
        }\
    } while(0)

#define ring_buffer_destroy(rb)\
    do {\
        log_process_type res = _ring_buffer_destroy((rb));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid ring buffer to destroy");\
            } break;\
            case INVALID_CAPACITY: {\
                FATAL("invalid ring buffer capacity");\
            } break;\
            case CORRECT: {\
                TRACE("ring buffer created");\
            } break;\
        }\
    } while(0)

#define ring_buffer_resize(rb)\
    do {\
        log_process_type res = _ring_buffer_resize((rb));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid ring buffer to resize");\
            } break;\
            case INVALID_CAPACITY: {\
                FATAL("ring buffer malloc failure");\
            } break;\
            case CORRECT: {\
                TRACE("ring buffer resized");\
            } break;\
        }\
    } while(0)

#define ring_buffer_push(rb, val)\
    do {\
        typeof(val) temp = val;\
        log_process_type res = _ring_buffer_push((rb), (&temp));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid ring buffer to push");\
            } break;\
            case OUT_OF_MEMORY: {\
                FATAL("ring buffer out of memory, please resize it");\
            } break;\
            case CORRECT: {\
                TRACE("ring buffer pushed '%s' back", #val);\
            } break;\
        }\
    } while(0)

#define ring_buffer_pop(rb, dest)\
    do {\
        log_process_type res = _ring_buffer_pop((rb), (dest));\
        switch(res) {\
            case NULL_PTR: {\
                FATAL("invalid ring buffer to pop");\
            } break;\
            case MEMORY_EMPTY: {\
                FATAL("ring buffer empty");\
            } break;\
            case CORRECT: {\
                TRACE("ring buffer popped back to '%s'", #dest);\
            } break;\
        }\
    } while(0)
