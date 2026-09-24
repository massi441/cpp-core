cmake_minimum_required(VERSION 3.15)
project(core_cpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # No compiler extensions

add_library(ml_core)

target_include_directories(ml_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_sources(ml_core PRIVATE ${SOURCES})

# Compiler stuff
if (MSVC)
    target_compile_options(ml_core PUBLIC /Zc:preprocessor) # for the recursion macros
endif ()

# Testing stuff
option(BUILD_CORE_TEST "Enable building the test project" OFF)

if (BUILD_CORE_TEST)
    enable_testing()
    add_subdirectory(tests)
endif()
