#include "platform/platform.h"

#include <stdlib.h>
#include <string.h>
void* platform_allocate(u64 size) {
    return malloc(size);
}

void* platform_allocate_aligned(u64 size, u64 alignment) {
    void* ptr = NULL;
    if(posix_memalign(&ptr, alignment, size) != 0) {
        return NULL;
    }
    return ptr;
}

void platform_free(void* block, b8 aligned) {
    (void)aligned;
    free(block);
}

void platform_zero_memory(void* block, u64 size) {
    memset(block, 0, size);
}

void platform_copy_memory(void* dest, const void* src, u64 size) {
    memcpy(dest, src, size);
}

void platform_move_memory(void* dest, const void* src, u64 size) {
    memmove(dest, src, size);
}

void platform_set_memory(void* dest, i32 value, u64 size) {
    memset(dest, value, size);
}
