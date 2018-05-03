CC=gcc

MPI=-DMPI
MPICC = mpic++

DEBUG=0
CFLAGS=-g -O3 -Wall -DDEBUG=$(DEBUG) -std=c++11
LDFLAGS= -lm

CFILES = flow-mpi.cpp graph-mpi.cpp cycletimer.c

all: flow-mpi

flow-mpi: $(CFILES)
	$(MPICC) $(CFLAGS) $(MPI) -o flow-mpi $(CFILES) $(LDFLAGS)

clean:
	rm -f flow-mpi