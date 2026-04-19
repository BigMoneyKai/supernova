file(GLOB_RECURSE TESTBED_SRC CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_executable(testbed ${TESTBED_SRC})

target_include_directories(testbed PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/engine/src
)

target_compile_options(testbed PRIVATE
    -g
    -fdeclspec
    -fcolor-diagnostics
)

target_compile_definitions(testbed PRIVATE
    _DEBUG
    SNIMPORT
)

target_link_libraries(testbed PRIVATE
    engine
)

set_target_properties(testbed PROPERTIES
    BUILD_RPATH "$ORIGIN"
)

