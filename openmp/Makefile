CC=g++ -std=c++11
OMP=-fopenmp -DOMP
CFLAGS=-g -O3 -Wall 
LDFLAGS= -lm

CFILES = graph-seq.cpp
HFILES = graph-seq.h

COMPFILES = flow-omp.cpp
HOMPFILES = 

all: crun-omp $(TURBO)

crun-omp: $(COMPFILES) $(HOMPFILES)
	$(CC) $(CFLAGS) $(OMP) -o flow-omp $(COMPFILES) $(LDFLAGS)
	
clean:
	rm -f *~ *.pyc
	rm -rf *.dSYM
	rm -f *.tgz
	rm -f flow-omp