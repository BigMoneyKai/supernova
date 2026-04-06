# Supernova

[中文说明](README_zh_CN.md)

Supernova is a small C engine skeleton with a lightweight testbed. The codebase is organized into a few focused modules under `engine/src`, plus a runnable demo under `testbed`.

## Modules

### `engine/src/core`
Core runtime utilities and engine services.

- `defines.h`: Cross-platform type aliases, compile-time asserts, platform/compiler detection, and export macros (`SNAPI`).
- `logger.h` / `logger.c`: Colored console logging, log levels, and a unified error/result enum (`log_process_type`).
- `asserts.h` / `asserts.c`: Assertion helpers built on the logging system.
- `memory.h` / `memory.c`: Central memory system entry points (init/shutdown, alloc/free, memset/memcpy, mem statistics) plus memory tags.
- `allocator.h`: Allocator implementations and APIs (linear arena, stack with mark/reset, fixed-size pool, and general tracked heap).
- `event.h` / `event.c`: Event system with a fixed-size payload (`event_context`) and typed event codes (keyboard, mouse, window, app).
- `input.h` / `input.c`: Keyboard/mouse state tracking, per-frame transitions, and conversion helpers.
- `app.h` / `app.c`: App lifecycle helpers (`app_create`, `app_run`) and configuration (`app_config`).

### `engine/src/container`
Container utilities used by the engine and testbed.

- `array.*`: Fixed-capacity array (push/insert/pop) with explicit capacity limits.
- `darray.*`: Resizable dynamic array (capacity doubles on resize).
- `adarray.*`: Allocation-determined dynamic array; similar API to `darray` but tied to allocator behavior.
- `ring_buffer.*`: Ring buffer with head/tail, dynamic resize, push/pop.

### `engine/src/platform`
Platform abstraction layer.

- `platform.h`: Window lifecycle, memory helpers, timing utilities, and event callback registration.
- `platform_linux.c`: Linux implementation.
- `platform_win32.c`: Windows implementation.

### `testbed`
Minimal application that exercises the engine.

- `testbed/src/main.c`: Initializes memory, runs allocator tests, wires input and window events, and starts the app loop.

### `bin`
Build outputs (example: `libengine.so` and `testbed`).

## Memory Tags
The memory system uses `memtag` to classify allocations and report usage.

| memtag | Allocator type | Usage |
| --- | --- | --- |
| `MEM_TAG_ARRAY` / `MEM_TAG_DARRAY` / `MEM_TAG_RING_BUFFER` | pool / general | container storage |
| `MEM_TAG_STRING` | linear / stack | short-lived string buffers |
| `MEM_TAG_JOB` | stack | temporary job allocations |
| `MEM_TAG_ENGINE` | linear | long-lived engine allocations |
| `MEM_TAG_TEXTURE` / `MEM_TAG_MATERIAL_INSTANCE` / `MEM_TAG_RENDERER` | general | rendering resources |
| `MEM_TAG_GAME` / `MEM_TAG_TRANSFORM` / `MEM_TAG_ENTITY` / `MEM_TAG_ENTITY_NODE` / `MEM_TAG_SCENE` | general / linear | game and scene data |

## Directory Layout

- `engine/` engine source and build scripts
- `testbed/` demo application
- `bin/` build outputs
- `build_all.sh` / `build_all.bat` full build scripts
