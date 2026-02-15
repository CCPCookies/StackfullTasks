cmake_minimum_required(VERSION 3.31)

project(stackfullTasks VERSION 1.0.0)

enable_language(C ASM_MASM)

set(INCLUDE_FILES
    include/Tasklet.h
    include/Task.h
    include/SimpleTask.h
)

set(SRC_FILES
    src/StackSwitch.asm
    src/Tasklet.cpp
    src/SimpleTask.cpp
)

add_library(stackfullTasks ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(stackfullTasks
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(stackfullTasks PRIVATE cxx_std_17)

option(BUILD_TESTING "Enable Test Project" OFF)
if (BUILD_TESTING)
    add_subdirectory(test)
endif ()

install(FILES ${INCLUDE_FILES} DESTINATION include)

install(TARGETS stackfullTasks
        PUBLIC_HEADER
          DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/include
)