CC=g++ -std=c++11

CFLAGS=-g -O3 -Wall
LDFLAGS=

CFILES = cycletimer.c flow-seq.cpp graph-seq.cpp
HFILES = cycletimer.h graph-seq.h

all: flow

flow: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o flow-seq $(CFILES) $(LDFLAGS)

clean:
	rm -f flow-seq
