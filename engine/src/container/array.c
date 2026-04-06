#include "array.h"
#include "core/memory.h"
#include "core/logger.h"

#include <stddef.h>

log_process_type _array_create(array* arr, u64 capacity, u64 stride) {
    if(!arr) {
        return NULL_PTR;
    }
    if(capacity == 0) {
        return INVALID_CAPACITY;
    }
    arr->capacity = capacity;
    arr->length = 0;
    arr->stride = stride;
    arr->data = snmalloc(capacity * stride, MEM_TAG_ARRAY);
    if(!arr->data) {
        return MALLOC_FAILED;
    }
    return CORRECT;
}

log_process_type _array_destroy(array* arr) {
    if(!arr) {
        return NULL_PTR;
    }
    snmfree(arr->data, arr->capacity * arr->stride, MEM_TAG_ARRAY);
    arr->data = NULL;
    arr->capacity = 0;
    arr->length = 0;
    arr->stride = 0;
    return CORRECT;
}

log_process_type _array_push(array* arr, const void* v_ptr) {
    if(!arr || !v_ptr) {
        return NULL_PTR;
    }
    if(arr->length == arr->capacity) {
        return OUT_OF_MEMORY;
    }
    u8* dest = (u8*)arr->data + arr->length * arr->stride;
    snmcopy(dest, v_ptr, arr->stride);
    arr->length++;

    return CORRECT;
}

log_process_type _array_insert(array* arr, u64 idx, const void* v_ptr) {
    if(!arr || !v_ptr) {
        return NULL_PTR;
    }

    if(idx > arr->length) {
        return OUT_OF_INDEX_BOUNDS;
    }

    if(arr->length == arr->capacity) {
        return OUT_OF_MEMORY;
    }

    u8* base = (u8*)arr->data;

    snmmove(base + (idx + 1) * arr->stride,
        base + idx * arr->stride,
        arr->stride * (arr->length - idx));
    snmcopy(base + idx * arr->stride, v_ptr, arr->stride);
    arr->length++;

    return CORRECT;
}

log_process_type _array_pop(array* arr, void* dest) {
    if (!arr || !arr->data) {
        return NULL_PTR; 
    }
    if(arr->length == 0) {
        return MEMORY_EMPTY;
    }   
    arr->length--;
    u8* src = (u8*)arr->data + arr->length * arr->stride;

    if (dest) {
        snmcopy(dest, src, arr->stride);
    }

    return CORRECT;

}

log_process_type _array_pop_at(array* arr, u64 idx, void* dest) {
    if(!arr || !arr->data) {
        return NULL_PTR;
    }

    if(arr->length == 0) {
        return MEMORY_EMPTY;
    }

    if(idx >= arr->length) {
        return OUT_OF_INDEX_BOUNDS;
    }

    u8* base = (u8*)arr->data;
    u8* target = base + idx * arr->stride;

    if(dest) {
        snmcopy(dest, target, arr->stride);
    }

    snmmove(target,
        target + arr->stride,
        (arr->length - idx - 1) * arr->stride);
    arr->length--;

    return CORRECT;
}
