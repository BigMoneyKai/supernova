#include "adarray.h"
#include "core/memory.h"
#include "core/logger.h"

#include <stddef.h>

log_process_type _adarray_create(adarray* arr, u64 capacity, u64 stride) {
    if(!arr) {
        return NULL_PTR;
    }
    if(capacity == 0) {
        return INVALID_CAPACITY;
    }
    arr->capacity = capacity;
    arr->length = 0;
    arr->stride = stride;
    arr->data = snmalloc(capacity * stride, MEM_TAG_DARRAY);
    if(!arr->data) {
        return MALLOC_FAILED;
    }
    return CORRECT;
}

log_process_type _adarray_destroy(adarray* arr) {
    if(!arr) {
        return NULL_PTR;
    }
    snmfree(arr->data, arr->capacity * arr->stride, MEM_TAG_DARRAY);
    arr->data = NULL;
    arr->capacity = 0;
    arr->length = 0;
    arr->stride = 0;
    return CORRECT;
}

log_process_type _adarray_resize(adarray* arr) {
    if (!arr) {
        return NULL_PTR;
    }

    u64 old_size = arr->capacity * arr->stride;
    u64 new_capacity = arr->capacity == 0 ? 1 : arr->capacity * ADARRAY_RESIZE_FACTOR;
    u64 new_size = new_capacity * arr->stride;

    void* new_mem = snmalloc(new_size, MEM_TAG_DARRAY);
    if (!new_mem) {
        return MALLOC_FAILED;
    }

    snmcopy(new_mem, arr->data, arr->capacity * arr->stride);

    snmfree(arr->data, old_size, MEM_TAG_DARRAY);

    arr->data = new_mem;
    arr->capacity = new_capacity;

    return CORRECT;
}

log_process_type _adarray_push(adarray* arr, const void* v_ptr) {
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

log_process_type _adarray_insert(adarray* arr, u64 idx, const void* v_ptr) {
    if(!arr || !v_ptr) {
        return NULL_PTR;
    }

    if(arr->length == arr->capacity) {
        return OUT_OF_MEMORY;
    }

    if(idx > arr->length) {
        return OUT_OF_INDEX_BOUNDS;
    }

    u8* base = (u8*)arr->data;

    snmmove(base + (idx + 1) * arr->stride,
        base + idx * arr->stride,
        arr->stride * (arr->length - idx));
    snmcopy(base + idx * arr->stride, v_ptr, arr->stride);
    arr->length++;

    return CORRECT;
}

log_process_type _adarray_pop(adarray* arr, void* dest) {
    if (!arr || arr->length == 0) {
        return NULL_PTR;
    }
    arr->length--;
    u8* src = (u8*)arr->data + arr->length * arr->stride;

    if (dest) {
        snmcopy(dest, src, arr->stride);
    }

    return CORRECT;

}

log_process_type _adarray_pop_at(adarray* arr, u64 idx, void* dest) {
    if(!arr || !arr->data) {
        return NULL_PTR;
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
