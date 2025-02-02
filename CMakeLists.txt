cmake_minimum_required(VERSION 3.10)

# Set the C and C++ compilers
set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/g++")

# Project name and version
project(ScriptScheduler VERSION 1.0)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

# Define source files
set(SOURCE_FILES
    "src/main.c"
    "src/scheduler.c"
    "src/utils/func.c"
    "src/priority_queue.c"
)

# Define test files
set(TEST_FILES
    "tests/unit/test_scheduler.c"
)

find_package(Threads REQUIRED)
find_package(json-c REQUIRED)

# Manually specify the paths for CUnit
set(CUNIT_INCLUDE_DIR /usr/include/CUnit)
set(CUNIT_LIB /usr/lib/x86_64-linux-gnu/libcunit.so)

# Include directories
include_directories(include ${CUNIT_INCLUDE_DIR})

# Add executable for the main application
add_executable(ScriptScheduler ${SOURCE_FILES})

# Link libraries for the main application
target_link_libraries(ScriptScheduler PRIVATE Threads::Threads json-c)

# Add executable for the test application
add_executable(test_scheduler ${TEST_FILES} src/scheduler.c src/utils/func.c src/priority_queue.c)

# Link libraries for the test application
target_link_libraries(test_scheduler PRIVATE ${CUNIT_LIB} Threads::Threads json-c)

# Handle compiler flags for MSVC
if(MSVC)
    target_compile_options(ScriptScheduler PRIVATE /Wall)
    set_property(TARGET ScriptScheduler PROPERTY 
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG>" STREQUAL "Debug")
        target_compile_options(ScriptScheduler PRIVATE /RTC1)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "$<CONFIG>" STREQUAL "Release")
        target_compile_options(ScriptScheduler PRIVATE /O2)
    endif()
else()
    # For non-MSVC compilers (e.g., GCC/Clang)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG>" STREQUAL "Debug")
        target_compile_options(ScriptScheduler PRIVATE -Wall -Wextra -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "$<CONFIG>" STREQUAL "Release")
        target_compile_options(ScriptScheduler PRIVATE -Wall -Wextra -O2)
    endif()
endif()