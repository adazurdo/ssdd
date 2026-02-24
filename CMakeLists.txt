cmake_minimum_required(VERSION 3.10)
project(practicas)

set(CMAKE_CXX_STANDARD 17)

add_executable(practicas main.cpp
        claves.h
        app-cliente.c
        app-cliente.h
        claves.c
        libclaves.c)

