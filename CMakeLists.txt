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

# Add compiler options
target_compile_options(script_scheduler PRIVATE -Wall -Wextra -O2)
