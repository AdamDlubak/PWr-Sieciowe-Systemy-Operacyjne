CFLAGS=-Wall -ansi -g

all: Zad1 Zad2 zip clean

zip: Lab2.zip

Zad1: Zad1.c
	$(CC) $(CFLAGS) -o $@ $^	

Zad2: Zad2.c
	$(CC) $(CFLAGS) -o $@ $^

Lab2.zip : *.c Makefile ReadMe.md
	zip $@ $^

clean:
	rm -f *.o $(TARGETS)
