cmake_minimum_required(VERSION 3.26)
project(5930_final C)

set(CMAKE_C_STANDARD 17)

add_executable(5930_final
        lc4.c
        lc4_disassembler.c
        lc4_disassembler.h
        lc4_loader.c
        lc4_loader.h
        lc4_memory.c
        lc4_memory.h)
