cmake_minimum_required(VERSION 3.5)
project(arm11_12)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(SOURCE_FILES
    src/emulate/definitions.h
    src/emulate/getCodes.c
    src/emulate/instructions.c
    src/emulate/emulate.c
    src/emulate/instructionCycle.c
    src/emulate/utilities.c
    src/emulate/setRegs.c
    src/emulate/shifter.c
        src/emulate/emulate.h)
# eventually just have emulate.c in source file

add_executable(arm11_12 ${SOURCE_FILES})