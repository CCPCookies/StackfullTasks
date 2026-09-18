cmake_minimum_required(VERSION 3.31)

project(stackfullTasks VERSION 1.0.0)

enable_language(C ASM_MASM)

set(INCLUDE_FILES
    include/Fibre.h
    include/Task.h
    include/SimpleTask.h
)

set(SRC_FILES
    src/StackSwitch.asm
    src/Fibre.cpp
    src/SimpleTask.cpp
)

add_library(stackfullTasks ${SRC_FILES} ${INCLUDE_FILES})

target_include_directories(stackfullTasks
        PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_compile_features(stackfullTasks PRIVATE cxx_std_17)

option(BUILD_TESTING "Enable Test Project" OFF)
if (BUILD_TESTING)
    add_subdirectory(test)
endif ()


install(
        TARGETS stackfullTasks
        EXPORT stackfullTasksTarget
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

install(FILES ${INCLUDE_FILES} DESTINATION include)

install(EXPORT stackfullTasksTarget
        DESTINATION share/stackfull-tasks
        FILE stackfull-tasksConfig.cmake
)