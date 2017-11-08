# Laboratorium 2

Stworzone w języku c z użyciem łącz nienazwanych - strumieni pipe.

#### Laboratorium nr 2 - 17.10.2017r.



## Makefile

Wykonuje kompilację wszystkich 2 zadań oraz spakowanie ich do archiwum `.zip`. 

**Wywołanie poprzez:**  

```bash
make all
```



## Zadanie 1

Zadanie to generuje listing na podstawie którego można sprawdzić pojemność strumienia `pipe` oraz ilość danych, które należy zwolnić aby móc ponownie zapisać do niego dane w przypadku jego przepełnienia. Z listingu znajdującego się poniżej wynika, iż należy zwolnić tyle miejsca w strumieniu, aby móc przesłać kolejną pełną wiadomość czyli w moim przykładzie pipe musi posiadać conajmniej 24 Kb wolnego miejsca aby móc ponownie wysłać kolejną wiadomość. Podczas prób udało się również wykazać, iż pojemność pipe to dokładnie 64 kb.

- [ ] > Wiadomość tę udało się uzyskać na podstawie dobierania odpowiednich wielkości wiadomości, tak aby był zauważalny przeskok (np. x wiadomości o rozmiarze 1 b).

**Wywołanie poprzez:**  

```bash
./Zad1 ilość-iteracji
```



**Przykładowy listing programu:**

```bash
./Zad1 20
        Child:  (Iter: 0)       I wrote  message (size: 24576 b)
        Child:  (Iter: 1)       I wrote  message (size: 24576 b)
        Parent: (Iter: 0)       I read message  (size: 4096 b)
        Parent: (Iter: 1)       I read message  (size: 4096 b)
        Child:  (Iter: 2)       I wrote  message (size: 24576 b)
        Parent: (Iter: 2)       I read message  (size: 4096 b)
        Parent: (Iter: 3)       I read message  (size: 4096 b)
        Parent: (Iter: 4)       I read message  (size: 4096 b)
        Parent: (Iter: 5)       I read message  (size: 4096 b)
        Parent: (Iter: 6)       I read message  (size: 4096 b)
        Parent: (Iter: 7)       I read message  (size: 4096 b)
        Child:  (Iter: 3)       I wrote  message (size: 24576 b)
        Parent: (Iter: 8)       I read message  (size: 4096 b)
        Parent: (Iter: 9)       I read message  (size: 4096 b)
        Parent: (Iter: 10)      I read message  (size: 4096 b)
        Parent: (Iter: 11)      I read message  (size: 4096 b)
        Parent: (Iter: 12)      I read message  (size: 4096 b)
        Parent: (Iter: 13)      I read message  (size: 4096 b)
        Child:  (Iter: 4)       I wrote  message (size: 24576 b)
        Parent: (Iter: 14)      I read message  (size: 4096 b)
        Parent: (Iter: 15)      I read message  (size: 4096 b)
        Parent: (Iter: 16)      I read message  (size: 4096 b)
        Parent: (Iter: 17)      I read message  (size: 4096 b)
        Parent: (Iter: 18)      I read message  (size: 4096 b)
        Parent: (Iter: 19)      I read message  (size: 4096 b)
        Child:  (Iter: 5)       I wrote  message (size: 20480 b)
        Child:  (Iter: 6)       I wrote everything what I could
        Child:  (Iter: 7)       I wrote everything what I could
        Child:  (Iter: 8)       I wrote everything what I could
        Child:  (Iter: 9)       I wrote everything what I could
        Child:  (Iter: 10)      I wrote everything what I could
        Child:  (Iter: 11)      I wrote everything what I could
        Child:  (Iter: 12)      I wrote everything what I could
        Child:  (Iter: 13)      I wrote everything what I could
        Child:  (Iter: 14)      I wrote everything what I could
        Child:  (Iter: 15)      I wrote everything what I could
        Child:  (Iter: 16)      I wrote everything what I could
        Child:  (Iter: 17)      I wrote everything what I could
        Child:  (Iter: 18)      I wrote everything what I could
        Child:  (Iter: 19)      I wrote everything what I could
```



## Zadanie 2

Program obsługuje dowolną ilość parametrów, gdzie poszczególne funkcje rozdzielone są znakiem ",".

> Jest to dokończony i poprawiony program z laboratorium nr 1, zadanie 4.

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
./Zad2 "sciezka_do_funkcji" "nazwa_funkcji" "parametry" "parametry" , "sciezka_do_kolejnej_funcki" "nazwa_kolejnej_funckji" "parametry_kolejnej_funckji"...
```



**Przykładowy listing programu:**

```bash
./Zad2 /bin/ls ls -l  , /bin/grep -l "\.c" , /bin/grep -l "Zad" , /bin/grep -l "4"
    -rwxrwx--- 1 root vboxsf  1614 Oct 17 12:43 Zad1.c
    -rwxrwx--- 1 root vboxsf  4095 Oct 12 17:30 Zad2.c
```