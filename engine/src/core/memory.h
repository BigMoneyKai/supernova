#pragma once

#include "defines.h"
#include "core/logger.h"

typedef struct stack_allocator stack_allocator;
typedef struct pool_allocator pool_allocator;
typedef struct linear_allocator linear_allocator;
typedef struct general_allocator general_allocator;

#define B(x) ((u64)(x))
#define KB(x) (B(x) * 1024ULL)
#define MB(x) (KB(x) * 1024ULL)

#define JOB_STACK_SIZE                MB(1)
#define STRING_STACK_SIZE             MB(1)

#define ARRAY_POOL_BLOCK_SIZE         B(32)
#define ARRAY_POOL_TOTAL_SIZE         KB(32)

#define DARRAY_POOL_BLOCK_SIZE        B(32)
#define DARRAY_POOL_TOTAL_SIZE        KB(32)

#define RING_POOL_BLOCK_SIZE          B(32)
#define RING_POOL_TOTAL_SIZE          KB(32)

#define BST_LINEAR_SIZE               MB(1)
#define ENGINE_LINEAR_SIZE            MB(2)

#define TEXTURE_HEAP_SIZE             MB(8)
#define MATERIAL_INSTANCE_HEAP_SIZE   MB(2)
#define RENDERER_HEAP_SIZE            MB(4)

#define GAME_LINEAR_SIZE              MB(2)
#define TRANSFORM_LINEAR_SIZE         MB(1)
#define ENTITY_LINEAR_SIZE            MB(1)
#define ENTITY_NODE_LINEAR_SIZE       MB(1)
#define SCENE_LINEAR_SIZE             MB(1)

typedef enum memtag {
    MEM_TAG_UNKNOWN,
    MEM_TAG_ARRAY,
    MEM_TAG_DARRAY,
    MEM_TAG_RING_BUFFER,
    MEM_TAG_BST,
    MEM_TAG_STRING,
    MEM_TAG_ENGINE,
    MEM_TAG_JOB,
    MEM_TAG_TEXTURE,
    MEM_TAG_MATERIAL_INSTANCE,
    MEM_TAG_RENDERER,
    MEM_TAG_GAME,
    MEM_TAG_TRANSFORM,
    MEM_TAG_ENTITY,
    MEM_TAG_ENTITY_NODE,
    MEM_TAG_SCENE,

    TAG_MAX_NUM,
} memtag;

typedef struct memsys_state {
    u64 total_size;
    u64 tagged_sizes[TAG_MAX_NUM];
} memsys_state;

extern memsys_state* mss;

// Allocators

// =============== Pool =================
extern pool_allocator array_pool32;
extern pool_allocator darray_pool32;
extern pool_allocator ring_buffer_pool32;
// ============== Linear ================
extern linear_allocator engine_linear;
// =============== Stack ================
extern stack_allocator job_stack;
extern stack_allocator string_stack;
// ============== General ===============
extern general_allocator array_general;
extern general_allocator darray_general;
extern general_allocator ring_buffer_general;
extern general_allocator bst_general;
extern general_allocator texture_general;
extern general_allocator material_instance_general;
extern general_allocator renderer_general;
extern general_allocator game_general;
extern general_allocator transform_general;
extern general_allocator entity_general;
extern general_allocator entity_node_general;
extern general_allocator scene_general;

SNAPI log_process_type _snminit(void* state);
SNAPI log_process_type _snmquit(void* state);
SNAPI log_process_type _snmalloc(void** out_block, u64 size, memtag tag);
SNAPI log_process_type _snmfree(void* block, u64 size, memtag tag);
SNAPI log_process_type _snmcopy(void* dest, const void* src, u64 size);
SNAPI log_process_type _snmmove(void* dest, const void* src, u64 size);
SNAPI log_process_type _snmset(void* block, i32 value, u64 size);
SNAPI log_process_type _snmzero(void* block, u64 size);
SNAPI log_process_type _snm_string_mark(u64* out_mark);
SNAPI log_process_type _snm_string_reset_to_mark(u64 mark);
SNAPI log_process_type _snm_string_reset(void);
SNAPI log_process_type _snm_job_mark(u64* out_mark);
SNAPI log_process_type _snm_job_reset_to_mark(u64 mark);
SNAPI log_process_type _snm_job_reset(void);

SNAPI log_process_type _get_meminfo(memtag tag, char* buffer, u64 bufsize);
SNAPI log_process_type _update_mstats_tui(void);

#define snminit(state) \
    do { \
        log_process_type _snm_res = _snminit((state)); \
        switch(_snm_res) { \
            case MEMSYS_NULL: { \
                FATAL("memory system state is null"); \
            } break; \
            case DOUBLE_INIT: { \
                WARN("memory system already initialized"); \
            } break; \
            case OUT_OF_MEMORY: { \
                FATAL("failed to initialize memory allocators"); \
            } break; \
            case CORRECT: { \
                TRACE("memory system initialized"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snmquit(state) \
    do { \
        log_process_type _snm_res = _snmquit((state)); \
        switch(_snm_res) { \
            case MEMSYS_NULL: { \
                FATAL("memory system state is null"); \
            } break; \
            case NON_INIT: { \
                WARN("memory system is not initialized"); \
            } break; \
            case CORRECT: { \
                TRACE("memory system shutdown"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snmalloc(size, tag) \
    ({ \
        void* _snm_ptr = 0; \
        u64 _snm_size = (size); \
        memtag _snm_tag = (tag); \
        log_process_type _snm_res = _snmalloc(&_snm_ptr, _snm_size, _snm_tag); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case NULL_PTR: { \
                FATAL("invalid output pointer for allocation"); \
            } break; \
            case INVALID_INPUT: { \
                if(_snm_size == 0) { \
                    FATAL("requested allocation size is 0"); \
                } else if(_snm_tag >= TAG_MAX_NUM) { \
                    FATAL("invalid memory tag"); \
                } else { \
                    FATAL("invalid allocation input"); \
                } \
            } break; \
            case OUT_OF_MEMORY: { \
                FATAL("memory allocation failure, tag=%u, size=%llu", (u32)_snm_tag, _snm_size); \
            } break; \
            case CORRECT: { \
                TRACE("memory allocated, tag=%u, size=%llu", (u32)_snm_tag, _snm_size); \
            } break; \
            default: break; \
        } \
        if(_snm_tag == MEM_TAG_UNKNOWN) { \
            WARN("memory allocated with MEM_TAG_UNKNOWN"); \
        } \
        _snm_ptr; \
    })

#define snmfree(block, size, tag) \
    do { \
        void* _snm_block = (block); \
        u64 _snm_size = (size); \
        memtag _snm_tag = (tag); \
        log_process_type _snm_res = _snmfree(_snm_block, _snm_size, _snm_tag); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case NULL_PTR: { \
                WARN("attempted to free null block"); \
            } break; \
            case INVALID_INPUT: { \
                if(_snm_tag == MEM_TAG_STRING || _snm_tag == MEM_TAG_JOB || _snm_tag == MEM_TAG_ENGINE || _snm_tag == MEM_TAG_UNKNOWN) { \
                    WARN("tag does not support individual free"); \
                } else if(_snm_tag >= TAG_MAX_NUM) { \
                    WARN("invalid memory tag"); \
                } else if(_snm_size == 0) { \
                    WARN("invalid free size"); \
                } else { \
                    WARN("invalid memory free request"); \
                } \
            } break; \
            case CORRECT: { \
                TRACE("memory freed, tag=%u, size=%llu", (u32)_snm_tag, _snm_size); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snmcopy(dest, src, size) \
    do { \
        log_process_type _snm_res = _snmcopy((dest), (src), (size)); \
        switch(_snm_res) { \
            case NULL_PTR: { \
                WARN("snmcopy received null pointer"); \
            } break; \
            case CORRECT: { \
                TRACE("snmcopy completed"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snmmove(dest, src, size) \
    do { \
        log_process_type _snm_res = _snmmove((dest), (src), (size)); \
        switch(_snm_res) { \
            case NULL_PTR: { \
                WARN("snmmove received null pointer"); \
            } break; \
            case CORRECT: { \
                TRACE("snmmove completed"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snmset(block, value, size) \
    do { \
        log_process_type _snm_res = _snmset((block), (value), (size)); \
        switch(_snm_res) { \
            case NULL_PTR: { \
                WARN("snmset received null block"); \
            } break; \
            case CORRECT: { \
                TRACE("snmset completed"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snmzero(block, size) \
    do { \
        log_process_type _snm_res = _snmzero((block), (size)); \
        switch(_snm_res) { \
            case NULL_PTR: { \
                WARN("snmzero received null block"); \
            } break; \
            case CORRECT: { \
                TRACE("snmzero completed"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snm_string_mark() \
    ({ \
        u64 _snm_mark = 0; \
        log_process_type _snm_res = _snm_string_mark(&_snm_mark); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case NULL_PTR: { \
                FATAL("invalid output pointer for string mark"); \
            } break; \
            case CORRECT: { \
                TRACE("string stack mark"); \
            } break; \
            default: break; \
        } \
        _snm_mark; \
    })

#define snm_string_reset_to_mark(mark) \
    do { \
        log_process_type _snm_res = _snm_string_reset_to_mark((mark)); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case CORRECT: { \
                TRACE("string stack reset to mark"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snm_string_reset() \
    do { \
        log_process_type _snm_res = _snm_string_reset(); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case CORRECT: { \
                TRACE("string stack reset"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snm_job_mark() \
    ({ \
        u64 _snm_mark = 0; \
        log_process_type _snm_res = _snm_job_mark(&_snm_mark); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case NULL_PTR: { \
                FATAL("invalid output pointer for job mark"); \
            } break; \
            case CORRECT: { \
                TRACE("job stack mark"); \
            } break; \
            default: break; \
        } \
        _snm_mark; \
    })

#define snm_job_reset_to_mark(mark) \
    do { \
        log_process_type _snm_res = _snm_job_reset_to_mark((mark)); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case CORRECT: { \
                TRACE("job stack reset to mark"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define snm_job_reset() \
    do { \
        log_process_type _snm_res = _snm_job_reset(); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case CORRECT: { \
                TRACE("job stack reset"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define get_meminfo(tag, buffer, bufsize) \
    do { \
        log_process_type _snm_res = _get_meminfo((tag), (buffer), (bufsize)); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case NULL_PTR: { \
                WARN("invalid memory info buffer"); \
            } break; \
            case INVALID_INPUT: { \
                WARN("invalid memory tag"); \
            } break; \
            case CORRECT: { \
                TRACE("memory info fetched"); \
            } break; \
            default: break; \
        } \
    } while(0)

#define update_mstats_tui() \
    do { \
        log_process_type _snm_res = _update_mstats_tui(); \
        switch(_snm_res) { \
            case NON_INIT: { \
                FATAL("memory system is not initialized"); \
            } break; \
            case CORRECT: { \
                TRACE("memory stats updated"); \
            } break; \
            default: break; \
        } \
    } while(0)
