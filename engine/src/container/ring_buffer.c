#include "ring_buffer.h"
#include "core/memory.h"
#include "core/logger.h"

#include <stddef.h>

log_process_type _ring_buffer_create(ring_buffer* rb, u64 capacity, u64 stride) {
    if(!rb) {
        return NULL_PTR;
    }
    if(capacity == 0) {
        return INVALID_CAPACITY;
    }
    rb->data = snmalloc(capacity * stride, MEM_TAG_RING_BUFFER);

    if(!rb->data) {
        return MALLOC_FAILED;
    }

    rb->head = 0;
    rb->tail = 0;
    rb->length = 0;
    rb->capacity = capacity;
    rb->stride = stride;
    return CORRECT;
}

log_process_type _ring_buffer_destroy(ring_buffer* rb) {
    if(!rb || !rb->data) {
        return NULL_PTR;
    }
    snmfree(rb->data, rb->capacity * rb->stride, MEM_TAG_RING_BUFFER);
    rb->data = NULL;
    rb->head = 0;
    rb->tail = 0;
    rb->length = 0;
    rb->capacity = 0;
    rb->stride = 0;
    return CORRECT;
}

log_process_type _ring_buffer_resize(ring_buffer* rb) {
    if(!rb || !rb->data) return NULL_PTR;

    u64 old_size = rb->capacity * rb->stride;
    u64 new_capacity = 2 * rb->capacity;
    u64 new_size = new_capacity * rb->stride;

    void* new_mem = snmalloc(new_size, MEM_TAG_RING_BUFFER);
    if(!new_mem) return MALLOC_FAILED;

    snmcopy(new_mem, rb->data, old_size);
    snmfree(rb->data, old_size, MEM_TAG_RING_BUFFER);

    rb->data = new_mem;
    rb->capacity = new_capacity;

    return CORRECT;
}

log_process_type _ring_buffer_push(ring_buffer* rb, const void* v_ptr) {
    if (!rb || !rb->data) return NULL_PTR;

    u8* dest = (u8*)rb->data + rb->tail * rb->stride;
    snmcopy(dest, v_ptr, rb->stride);

    rb->tail = (rb->tail + 1) % rb->capacity;

    if (rb->length < rb->capacity) {
        rb->length++;
    } else {
        return OUT_OF_MEMORY;

        // rb->head = (rb->head + 1) % rb->capacity;
    }

    return CORRECT;
}

log_process_type _ring_buffer_pop(ring_buffer* rb, void* dest) {
    if(!rb || !rb->data) return NULL_PTR;
    if(rb->length == 0) return MEMORY_EMPTY;

    u8* src = (u8*)rb->data + rb->head * rb->stride;
    if(dest) {
        snmcopy(dest, src, rb->stride);
    }
    rb->head = (rb->head + 1) % rb->capacity;
    rb->length--;

    return CORRECT;
}
