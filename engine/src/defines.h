#pragma once

#include <stdint.h>

// platform compatibility
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define SN_PLATFORM_WINDOWS 1
    #ifndef _WIN64
        #error "64-bit is required on Windows."
    #endif

#elif defined(__APPLE__)
    #define SN_PLATFORM_APPLE 1
    #include <TargetConditional.h>
    #if TARGET_OS_IPHONE
        #define SN_PLATFORM_IOS 1
    #elif TARGET_OS_MAC
        #define SN_PLATFORM_MAC 1
    #else
        #error "Other unsupported operating system."
    #endif

#elif defined(__linux__) || defined(__gnu_linux__)
    #define SN_PLATFORM_LINUX 1
    #if defined(__ANDROID__)
        #define SN_PLATFORM_ANDROID 1
    #endif

#elif defined(__unix__)
    #define SN_PLATFORM_UNIX 1
    #if defined(_POSIX_VERSION)
        #define SN_PLATFORM_POSIX 1
    #endif

#else
    #error "Unknown/Unsupported operating system."
#endif

// Ensure all platform macros are defined for compatibility.
#ifndef SN_PLATFORM_WINDOWS
    #define SN_PLATFORM_WINDOWS 0
#endif
#ifndef SN_PLATFORM_APPLE
    #define SN_PLATFORM_APPLE 0
#endif
#ifndef SN_PLATFORM_IOS
    #define SN_PLATFORM_IOS 0
#endif
#ifndef SN_PLATFORM_MAC
    #define SN_PLATFORM_MAC 0
#endif
#ifndef SN_PLATFORM_LINUX
    #define SN_PLATFORM_LINUX 0
#endif
#ifndef SN_PLATFORM_ANDROID
    #define SN_PLATFORM_ANDROID 0
#endif
#ifndef SN_PLATFORM_UNIX
    #define SN_PLATFORM_UNIX 0
#endif
#ifndef SN_PLATFORM_POSIX
    #define SN_PLATFORM_POSIX 0
#endif

// compiler detection
#if defined(_MSC_VER)
    #define SN_COMPILER_MSVC 1
#elif defined(__clang__)
    #define SN_COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define SN_COMPILER_GCC 1
#endif

// cross-platform compatibility
#if defined(_MSC_VER)
    #define SN_RESTRICT __restrict
    #define SN_ALIGN(x) __declspec(align(x))
    #include <intrin.h>
    #define SN_BREAK() __debugbreak()
    #define SN_INLINE __forceinline
    #define SN_STATIC_ASSERT static_assert
    #define SN_COMPILER_BARRIER() _ReadWriteBarrier()
    #define SN_CPU_BARRIER() MemoryBarrier()
#elif defined(__GNUC__) || defined(__clang__)
    #define SN_RESTRICT restrict
    #define SN_ALIGN(x) __attribute__((aligned(x)))
    #define SN_BREAK() __builtin_debugtrap()
    #if defined(SN_DEBUG)
        #define SN_INLINE inline
    #else
        #define SN_INLINE static inline __attribute__((always_inline))
    #endif
    #if __STDC_VERSION__ >= 201112L
        #define SN_STATIC_ASSERT _Static_assert
    #else
        #define SN_STATIC_ASSERT(cond, msg) typedef char static_assertion_##__LINE__[(cond)?1:-1]
    #endif
    #define SN_COMPILER_BARRIER() __asm__ __volatile__("" ::: "memory")
    #define SN_CPU_BARRIER() __atomic_thread_fence(__ATOMIC_SEQ_CST)
#else
    #define SN_RESTRICT
    #define SN_ALIGN(x) (void(0))
    #define SN_BREAK() (void(0))
    #define SN_INLINE
    #define SN_STATIC_ASSERT
    #define SN_COMPILER_BARRIER() (void(0))
    #define SN_CPU_BARRIER() (void(0))
#endif

#define SN_MEM_BARRIER() \
    do { SN_COMPILER_BARRIER(); SN_CPU_BARRIER(); } while(0)

// unsigned int
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// signed int
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// float point types
typedef float f32;
typedef double f64;

// boolean types
typedef uint32_t b32;
typedef uint8_t b8;

// address types
typedef uintptr_t addr;

// type assertions
SN_STATIC_ASSERT(sizeof(u8) == 1, "Exptypeofected u8 to be 1 byte.");
SN_STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 byte.");
SN_STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 byte.");
SN_STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 byte.");

SN_STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");
SN_STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 byte.");
SN_STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 byte.");
SN_STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 byte.");

SN_STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 byte.");
SN_STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 byte.");

SN_STATIC_ASSERT(sizeof(b32) == 4, "Expected b32 to be 4 byte.");
SN_STATIC_ASSERT(sizeof(b8) == 1, "Expected b8 to be 1 byte.");

SN_STATIC_ASSERT(sizeof(addr) == sizeof(void*), "Expected addr to match pointer size.");

// define boolean values
#define SN_TRUE 1
#define SN_FALSE 0

// .dll/.so
#if defined(_MSC_VER)
    #if defined(SN_EXPORT)
        #define SNAPI __declspec(dllexport)
    #elif defined(SN_IMPORT)
        #define SNAPI __declspec(dllimport)
    #else
        #define SNAPI
    #endif
#elif defined(__GNUC__) || defined(__clang__)
    #define SNAPI __attribute__((visibility("default")))
#else
    #define SNAPI
#endif

// API choice
#if SN_PLATFORM_WINDOWS
    #define SN_RENDER_API_D3D11
#elif SN_PLATFORM_APPLE
    #define SN_RENDER_API_METAL
#elif SN_PLATFORM_LINUX
    #define SN_RENDER_API_VULKAN
#else
    #define SN_RENDER_API_OPENGL
#endif
