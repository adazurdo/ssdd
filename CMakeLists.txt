cmake_minimum_required(VERSION 3.10)
project(practicas)

set(CMAKE_CXX_STANDARD 17)

add_library(claves SHARED claves.c claves.h)

target_compile_options(claves PRIVATE -fPIC)

add_executable(practicas main.c)

target_link_libraries(practicas PRIVATE claves pthread rt)

target_link_directories(practicas PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
