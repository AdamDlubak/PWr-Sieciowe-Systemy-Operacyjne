CFLAGS=-Wall -g -std=c99 

all: Zad1-Client Zad1-Server clean

Zad1-Client: Zad1-Client.c
	$(CC) $(CFLAGS) -o $@ $^	

Zad1-Server: Zad1-Server.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGETS)
