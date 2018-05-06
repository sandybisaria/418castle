CC=gcc

MPI=-DMPI
MPICC = mpic++

DEBUG=0
CFLAGS=-g -O3 -Wall -DOMPI_SKIP_MPICXX -std=c++11
LDFLAGS= -lm

CFILES = graph-mpi.cpp flow-mpi.cpp cycletimer.c
HFILES = cycletimer.h graph-mpi.h

all: flow-mpi

flow-mpi: $(CFILES) $(XCFILES) $(HFILES) $(XHFILES)
	$(MPICC) $(CFLAGS) $(MPI) -o flow-mpi $(CFILES) $(XCFILES) $(LDFLAGS)

clean:
	rm -f flow-mpi