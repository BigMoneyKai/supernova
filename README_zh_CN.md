# Supernova

[English Version](README.md)

Supernova 是一个精简的 C 引擎骨架和测试程序。代码主要集中在 `engine/src` 下的模块，`testbed` 提供了一个可运行的示例。

## 模块说明

### `engine/src/core`
核心运行时与引擎服务。

- `defines.h`: 跨平台类型别名、编译期断言、平台/编译器检测、导出宏（`SNAPI`）。
- `logger.h` / `logger.c`: 彩色控制台日志、日志级别与统一错误/结果码（`log_process_type`）。
- `asserts.h` / `asserts.c`: 基于日志系统的断言辅助。
- `memory.h` / `memory.c`: 内存系统入口（初始化/退出、分配/释放、内存拷贝与统计）及内存标签。
- `allocator.h`: 分配器实现与接口（线性 arena、带 mark/reset 的栈式、固定块的池式，以及通用分配器）。
- `event.h` / `event.c`: 事件系统，固定大小的 `event_context` 载荷与键鼠/窗口/应用事件码。
- `input.h` / `input.c`: 键盘/鼠标状态维护、按键状态转变与辅助转换。
- `app.h` / `app.c`: 应用生命周期（`app_create` / `app_run`）与配置（`app_config`）。

### `engine/src/container`
引擎与测试程序使用的容器工具。

- `array.*`: 固定容量数组（push/insert/pop）。
- `darray.*`: 可扩容动态数组（容量按倍数增长）。
- `adarray.*`: 由分配器行为决定的动态数组，接口与 `darray` 类似。
- `ring_buffer.*`: 环形缓冲区（head/tail、可扩容、push/pop）。

### `engine/src/platform`
平台抽象层。

- `platform.h`: 窗口生命周期、内存辅助、计时功能与事件回调注册。
- `platform_linux.c`: Linux 实现。
- `platform_win32.c`: Windows 实现。

### `testbed`
用于演示与验证的最小应用。

- `testbed/src/main.c`: 初始化内存系统、执行分配器测试、注册输入与窗口事件并进入主循环。

### `bin`
构建输出（例如 `libengine.so` 与 `testbed`）。

## 内存标签
内存系统使用 `memtag` 分类分配并统计使用量。

| memtag | 分配器类型 | 用途 |
| --- | --- | --- |
| `MEM_TAG_ARRAY` / `MEM_TAG_DARRAY` / `MEM_TAG_RING_BUFFER` | pool / general | 容器存储 |
| `MEM_TAG_STRING` | linear / stack | 短生命周期字符串缓冲 |
| `MEM_TAG_JOB` | stack | 临时任务内存 |
| `MEM_TAG_ENGINE` | linear | 引擎长期内存 |
| `MEM_TAG_TEXTURE` / `MEM_TAG_MATERIAL_INSTANCE` / `MEM_TAG_RENDERER` | general | 渲染资源 |
| `MEM_TAG_GAME` / `MEM_TAG_TRANSFORM` / `MEM_TAG_ENTITY` / `MEM_TAG_ENTITY_NODE` / `MEM_TAG_SCENE` | general / linear | 游戏与场景数据 |

## 目录结构

- `engine/` 引擎源码与构建脚本
- `testbed/` 示例应用
- `bin/` 构建产物
- `build_all.sh` / `build_all.bat` 全量构建脚本
