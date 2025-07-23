cmake_minimum_required(VERSION 3.10)
project(GarbageCollector C)

set(CMAKE_C_STANDARD 99)

set(SOURCES
    testCases.c
    vm.c
    sneknew.c
    snekobject.c
    stack.c
)

add_executable(GarbageCollector ${SOURCES})

target_compile_options(GarbageCollector PRIVATE -Wall -Wextra -Wpedantic)