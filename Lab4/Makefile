CFLAGS=-c -Wall -ansi -g -std=gnu99

all: InitScript Zad1 clean


Zad1: Zad1.o SMLibrary.o SemLibrary.o
	$(CC) $(CDFLAGS) -o $@ $^

InitScript.o: InitScript.c
	$(CC) $(CFLAGS) -c -o $@ $<

Zad1.o: Zad1.c
	$(CC) $(CFLAGS) -c -o $@ $<

SMLibrary.o: SMLibrary.c SMLibrary.h
	$(CC) $(CFLAGS) -c -o $@ $<

SemLibrary.o: SemLibrary.c SemLibrary.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o $(TARGETS)
