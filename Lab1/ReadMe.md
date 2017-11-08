# Laboratorium 1

Stworzone w języku c z użyciem wielu procesów -fork() oraz strumieni przesyłania danych pipe()

#### Laboratorium nr 1 - 10.10.2017r.



## Makefile

Wykonuje kompilację wszystkich 4 zadań oraz spakowanie ich do archiwum `.zip`. 

**Wywołanie poprzez:**  

```bash
make all
```



## Zadanie 1

Utworzenie procesu potomnego poprzez fork. Wyświetlenie PID i PPID rodzica i potomka.

**Wywołanie poprzez:**  

```bash
./Zad1
```



**Przykładowy listing programu:**

```bash
./Zad1
Parent:
        PID:  2888
        PPID: 2596
Child:
        PID:  2889
        PPID: 2888

```



## Zadanie 2

Utworzenie procesu potomnego poprzez fork. Utworzenie strumienia pipe() i przesłanie informacji od potomka do rodzica poprzez write() i read().

**Wywołanie poprzez:**  

```bash
./Zad2
```



**Przykładowy listing programu:**

```bash
./Zad2
Child:   I sent very important message to my parent!
Parent:  My child told me PID number: 2905

```



## Zadanie 3

Utworzenie procesu potomnego poprzez fork. Utworzenie strumienia pipe(), przekierowanie strumienia wyjścia potomka oraz wejścia rodzica na dwa osobne potoki oraz przesłanie informacji wykonany poprzez operacje zdefiniowane w funkcji execl().

**Wywołanie poprzez:**  

```bash
./Zad3
```



**Przykładowy listing programu:**

```bash
./Zad3
     1  total 56
     2  -rwxrwx--- 1 root vboxsf   307 lis  8 11:01 Makefile
     3  -rwxrwx--- 1 root vboxsf  2012 paź 12 17:28 ReadMe.md
     4  -rwxrwx--- 1 root vboxsf 10096 paź 12 17:29 Zad1
     5  -rwxrwx--- 1 root vboxsf   635 paź 11 15:18 Zad1.c
     6  -rwxrwx--- 1 root vboxsf 10400 lis  8 11:13 Zad2
     7  -rwxrwx--- 1 root vboxsf   799 paź 17 12:04 Zad2.c
     8  -rwxrwx--- 1 root vboxsf 10248 paź 12 17:29 Zad3
     9  -rwxrwx--- 1 root vboxsf   835 paź 11 15:21 Zad3.c
    10  -rwxrwx--- 1 root vboxsf 14992 lis  8 11:13 Zad4
    11  -rwxrwx--- 1 root vboxsf  4095 paź 12 17:30 Zad4.c

```



## Zadanie 4

Program obsługuje dowolną ilość parametrów, gdzie poszczególne funkcje rozdzielone są znakiem ",".

**Skrócona zasada działania programu:**

1. Zliczanie liczby funkcji do wykonania

2. Utworzenie odpowiedniej liczby potoków (pipe'ów)

3. Ostateczne parsowanie parametrów i wykonywanie poleceń:
   a) Podzielenie funkcji na 3 scenariusze:

   - Pierwsza funkcja, gdzie dane są jedynie wysyłane do strumienia.
   - Ostatnia funkcja, gdzie dane są jedynie odbierane ze strumienia i wyświetlane na standardowe  wyjście.
   - Funkcje pośrednie, gdzie zarówno dane są przyjmowane z jednego potoku jak i wysyłane (przetworzone) do kolejnego.

   b) Weryfikowanie rodzaju parametru: funkcja, parametr funkcji czy znak rozdzielający (",").
   c) Tworzenie procesów potomnych.

4. Zamknięcie wszystkich potoków.

5. Zwalnianie pamięci zajmowanych przez tablice potoków.

6. Oczekiwanie na zakończenie procesów.

7. Zakończenie działania programu.

**Wywołanie poprzez:**  

```bash
./Zad4 "sciezka_do_funkcji" "nazwa_funkcji" "parametry" "parametry" , "sciezka_do_kolejnej_funcki" "nazwa_kolejnej_funckji" "parametry_kolejnej_funckji"...
```



**Przykładowy listing programu:**

```bash
./Zad4 "/bin/ls" "ls" "-l"  , "/bin/grep" "-l" "\.c" , "/bin/grep" "-l" "Zad" , "/bin/grep" "-l" "4"
-rwxrwx--- 1 root vboxsf   799 paź 17 12:04 Zad2.c
-rwxrwx--- 1 root vboxsf  4095 paź 12 17:30 Zad4.c

```