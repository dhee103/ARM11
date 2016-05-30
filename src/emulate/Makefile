CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o .h

.PHONY: all clean

all: emulate

emulate: emulate.o getCodes.o instructionCycle.o instructions.o shifter.o utilities.o

emulate.o: getCodes.h utilities.h

getCodes.o: getCodes.h instructions.h instructionCycle.h

instructionCycle.o: instructionCycle.h getCodes.h utilities.h

instructions.o: instructions.h getCodes.h shifter.h

shifter.o: shifter.h

utilities.o: utilities.h

clean:
	rm -f $(wildcard *.o)
	rm -f emulate
