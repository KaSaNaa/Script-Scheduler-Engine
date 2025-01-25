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
    src/main.c
    src/scheduler.c
    src/utils/func.c
)

find_package(Threads REQUIRED)
find_package(json-c REQUIRED)

# Include directories
include_directories(include)
include_directories("C:/msys64/mingw64/include")

# Add executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link pthread library
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads json-c)

# Handle compiler flags for MSVC
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall)
    set_property(TARGET ${PROJECT_NAME} PROPERTY 
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG>" STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE /RTC1)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "$<CONFIG>" STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE /O2)
    endif()
else()
    # For non-MSVC compilers (e.g., GCC/Clang)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR "$<CONFIG>" STREQUAL "Debug")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -g)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR "$<CONFIG>" STREQUAL "Release")
        target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
    endif()
endif()