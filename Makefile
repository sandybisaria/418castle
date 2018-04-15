CC=g++ -m64

CFLAGS=-g -O3 -Wall
LDFLAGS= -lm

CFILES = main.cpp
HFILES = 

all: flow

flow: $(CFILES) $(HFILES) 
	$(CC) $(CFLAGS) -o flow $(CFILES) $(LDFLAGS)

clean:
	rm -f flow
