CC=g++ -std=c++11
OMP=-fopenmp -DOMP
CFLAGS=-g -O3 -Wall 
LDFLAGS= -lm

CFILES = graph-omp.cpp bmp-omp.cpp cycletimer.c
HFILES = graph-omp.h cyclertimer.h

all: crun-omp

crun-omp: $(CFILES)
	$(CC) $(CFLAGS) $(OMP) -o bmp-omp $(CFILES) $(LDFLAGS)
	
clean:
	rm -f bmp-omp