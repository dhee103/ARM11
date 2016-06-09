CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o .h

.PHONY: all clean

all: emulate

emulate: emulate.o instructionCycle.o instructions.o utilities.o

emulate.o: utilities.h

instructionCycle.o: instructionCycle.h utilities.h

instructions.o: instructions.h

utilities.o: utilities.h

clean:
	rm -f $(wildcard *.o)
	rm -f emulate
