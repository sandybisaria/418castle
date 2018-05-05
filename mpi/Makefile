CC=gcc

MPI=-DMPI
MPICC = mpicc

DEBUG=0
CFLAGS=-g -O3 -Wall
LDFLAGS= -lm

CFILES = graph-mpi.cpp flow-mpi.cpp cycletimer.c
HFILES = cycletimer.h graph-mpi.h

all: flow-mpi

flow-mpi: $(CFILES) $(XCFILES) $(HFILES) $(XHFILES)
	$(MPICC) $(CFLAGS) $(MPI) -o flow-mpi $(CFILES) $(XCFILES) $(LDFLAGS)

clean:
	rm -f flow-mpi