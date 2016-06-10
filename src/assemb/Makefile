CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o .h

.PHONY: all clean

all: assemble 

assemble: run.o branch.o data_process.o definitions.o multiply.o single_data_process.o special.o ST.o

assemble.o: run.o
 



clean:
	rm -f $(wildcard *.o)
	rm -f assemble

