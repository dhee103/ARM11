cmake_minimum_required(VERSION 3.5)
project(arm11_12)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")

set(EMULATE_SOURCE_FILES
    src/emulate/definitions.h
    src/emulate/instructions.c
    src/emulate/emulate.c
    src/emulate/instructionCycle.c
    src/emulate/utilities.c
        src/assemble/run.h)
# eventually just have emulate.c in source file

add_executable(arm11_12_emulate ${EMULATE_SOURCE_FILES})

#set(ASSEMBLE_SOURCE_FILES
#        src/assemble/assemble.c
#        src/assemble/definitions.h
#        src/assemble/fileUtils.h
#        src/assemble/fileUtils.c
#        src/assemble/dataProcessing.c)
#
#add_executable(arm11_12_assemble ${ASSEMBLE_SOURCE_FILES})