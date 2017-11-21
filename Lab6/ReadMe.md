# Symulacja obsługi serwera plików FTP

Stworzone w języku c z użyciem połączenia TCP i socketów.

#### Laboratorium nr 6 - 21.11.2017r.

Wylistowanie katalogu
pobranie pliku
rozłączenie
Za pomocą TCP

## Makefile

Wykonuje kompilację wszystkich zadań. 

Wywołanie poprzez:  

```bash
make all
```

## Zad1

Program składa się z dwóch podprogramów, gdzie każdy z nich należy uruchomić osobno. Są to:

- Zad1-Client - odpowiedzialny za symulowanie pracy klienta poprzez np. wysyłanie żadań do serwera.
- Zad1-Server - serwer aplikacji, ciągle działający, oczekujący na połączenie z klientem i wykonanie zleconych przez niego zadań.



Obydwa programy można uruchomić bezparametrowo z domyślnymi wartościami: `Hostname: 127.0.0.1` oraz `Port: 57500` lub poprzez podanie odpowiednixh parametrów uruchomieniowych:

```bash
./Zad1-Client hostanme port
./Zad1-Server port
```



**Przykładowy Listing programu:**

```bash


```

