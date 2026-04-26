#include "platform/platform.h"

#if SN_PLATFORM_WINDOWS

#include <malloc.h>
#include <stdlib.h>
#include <string.h>

void* platform_allocate(u64 size) {
    return malloc((size_t)size);
}

void* platform_allocate_aligned(u64 size, u64 alignment) {
    return _aligned_malloc((size_t)size, (size_t)alignment);
}

void platform_free(void* block, b8 aligned) {
    if (aligned) {
        _aligned_free(block);
    } else {
        free(block);
    }
}

void platform_zero_memory(void* block, u64 size) {
    memset(block, 0, (size_t)size);
}

void platform_copy_memory(void* dest, const void* src, u64 size) {
    memcpy(dest, src, (size_t)size);
}

void platform_move_memory(void* dest, const void* src, u64 size) {
    memmove(dest, src, (size_t)size);
}

void platform_set_memory(void* dest, i32 value, u64 size) {
    memset(dest, value, (size_t)size);
}

#endif
