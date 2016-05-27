CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o .h

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

emulate: emulate.o

emulate.o: definitions.h getCodes.h utilities.h


clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
