CFLAGS=-c -Wall -ansi -g -std=gnu99 

all: Zad1 clean

Zad1: Zad1.o
	$(CC) $(CDFLAGS) -pthread -o $@ $^

Zad1.o: Zad1.c
	$(CC) $(CFLAGS) -pthread -c -o $@ $<

clean:
	rm -f *.o $(TARGETS)
