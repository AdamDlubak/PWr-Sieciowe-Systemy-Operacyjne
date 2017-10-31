CFLAGS=-Wall -ansi -g -std=gnu99

all: Library Zad1 clean

Zad1: Zad1.c
	$(CC) $(CFLAGS) -o $@ $^	

Library: library.c library.h
	$(CC) $(CFLAGS) library.c -c -o library.o

clean:
	rm -f *.o $(TARGETS)
