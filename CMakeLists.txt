cmake_minimum_required(VERSION 3.29)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(C_COMPILER gcc)
set(CMAKE_C_COMPILER gcc)

project(win_ui VERSION 1.0 LANGUAGES C)

file(GLOB sources "src/*.c")
set(CMAKE_C_FLAGS "-g")
add_executable(win_ui ${sources})
target_include_directories(win_ui PUBLIC "./include")
