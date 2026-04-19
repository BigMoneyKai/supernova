cmake_minimum_required(VERSION 3.26)

include(cmake/platform.cmake)
include(cmake/dependencies.cmake)

project(supernova VERSION 1.0.0)
platform_setup()
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

packages_setup()

add_subdirectory(engine)
add_subdirectory(testbed)
