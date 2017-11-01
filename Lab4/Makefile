CFLAGS=-c -Wall -ansi -g -std=gnu99

all: Zad1 clean


Zad1: Zad1.o SHLibrary.o
	$(CC) $(LD_FLAGS) -o $@ $^

Zad1.o: Zad1.c
	$(CC) $(CFLAGS) -c -o $@ $<

SHLibrary.o: SHLibrary.c SHLibrary.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGETS)
