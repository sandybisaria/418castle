CC=g++ -std=c++11
OMP=-fopenmp -DOMP
CFLAGS=-g -O3 -Wall 
LDFLAGS= -lm

CFILES = graph-omp.cpp flow-omp.cpp cycletimer.c
HFILES = graph-omp.h cyclertimer.h

all: crun-omp

crun-omp: $(CFILES)
	$(CC) $(CFLAGS) $(OMP) -o flow-omp $(CFILES) $(LDFLAGS)
	
clean:
	rm -f flow-omp