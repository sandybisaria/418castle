CC=g++ -std=c++11

CFLAGS=-g -O3 -Wall
LDFLAGS=

CFILES = cycletimer.c bmp-seq.cpp graph-seq.cpp
HFILES = cycletimer.h graph-seq.h

all: flow

flow: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o bmp-seq $(CFILES) $(LDFLAGS)

clean:
	rm -f bmp-seq
