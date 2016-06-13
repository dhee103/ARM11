CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o .h

.PHONY: all clean

all: emulate assemble

emulate: emulate.o instructionCycle.o instructions.o utilities.o

emulate.o: utilities.h

instructionCycle.o: instructionCycle.h utilities.h

instructions.o: instructions.h

utilities.o: utilities.h

assemble: assemble.o assembler_instructions.o run.o

assemble.o: run.h

run.o: run.h assembler_instructions.h

assembler_instructions.o: assembler_instructions.h



clean:
	rm -f $(wildcard *.o)
	rm -f emulate
