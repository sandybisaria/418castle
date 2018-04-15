CC=g++

CFLAGS=-g -O3 -Wall
LDFLAGS=

CFILES = flow-seq.cpp graph-seq.cpp
HFILES = graph-seq.h

all: flow

flow: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o flow-seq $(CFILES) $(LDFLAGS)

clean:
	rm -f flow-seq
