cmake_minimum_required(VERSION 3.31)

project(stackfullTasks VERSION 1.0.0)

enable_language(C ASM_MASM)

set(SRC_FILES
    src/StackSwitch.asm
    src/Tasklet.cpp
    include/Tasklet.h
    include/Task.h
)

add_library(stackfullTasks ${SRC_FILES})

target_include_directories(stackfullTasks
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(stackfullTasks PRIVATE cxx_std_17)

add_subdirectory(test)