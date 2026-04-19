file(GLOB_RECURSE ENGINE_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(engine SHARED ${ENGINE_SRC})
target_compile_options(engine PRIVATE
    -Wno-gnu-zero-variadic-macro-arguments
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
)

target_compile_options(engine PRIVATE
    -g
    -fPIC
    -fdeclspec
    -fcolor-diagnostics
)

target_compile_definitions(engine PRIVATE
    _DEBUG
    SNEXPORT
)

target_link_libraries(engine PRIVATE
    Vulkan::Vulkan
    xcb
    X11-xcb
    xkbcommon
)
