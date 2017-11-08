CFLAGS=-Wall -ansi -g

all: Zad1 Zad2 Zad3 Zad4 zip clean

zip: Lab1.zip

Zad1: Zad1.c
	$(CC) $(CFLAGS) -o $@ $^	

Zad2: Zad2.c
	$(CC) $(CFLAGS) -o $@ $^

Zad3: Zad3.c
	$(CC) $(CFLAGS) -o $@ $^

Zad4: Zad4.c
	$(CC) $(CFLAGS) -o $@ $^

Lab1.zip : *.c Makefile ReadMe.md
	zip $@ $^

clean:
	rm -f *.o $(TARGETS)
