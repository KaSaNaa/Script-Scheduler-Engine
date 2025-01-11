cmake_minimum_required(VERSION 3.10)

# Project name and version
project(ScriptScheduler VERSION 1.0)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define source files
set(SOURCE_FILES
    src/main.c
    src/scheduler.c
)

# Include directories
include_directories(include)

# Add executable
add_executable(script_scheduler ${SOURCE_FILES})

# Handle compiler flags for MSVC
if(MSVC)
    target_compile_options(script_scheduler PRIVATE /Wall)
    set_property(TARGET script_scheduler PROPERTY 
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG>" STREQUAL "Debug")
        target_compile_options(script_scheduler PRIVATE /RTC1)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "$<CONFIG>" STREQUAL "Release")
        target_compile_options(script_scheduler PRIVATE /O2)
    endif()
else()
    # For non-MSVC compilers (e.g., GCC/Clang)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG>" STREQUAL "Debug")
        target_compile_options(script_scheduler PRIVATE -Wall -Wextra -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "$<CONFIG>" STREQUAL "Release")
        target_compile_options(script_scheduler PRIVATE -Wall -Wextra -O2)
    endif()
endif()
