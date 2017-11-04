# Symulacja systemu bankowego

Stworzone w języku c z użyciem semaforów i pamięci współdzielonej.

#### Laboratorium nr 4 - 31.10.2017r.



## Makefile

Wykonuje kompilację wszystkich zadań. 

Wywołanie poprzez:  

```bash
make all
```



## InitScript

Skrypt odpowiadający za uruchomienie wszystkich zadań w wielu procesach. Zawiera parametry przekazywane do programu `Zad1`.  

Wywołanie poprzez

```bash
./InitScript liczba_procesów
```

np.

```bash
./InitScript 15
```



## Zad1

Zasadnicza część programu. Zawiera dołączone biblioteki (`SemLibrary`, `SMLibrary`)zawierające funkcje obsługujące zadania na semaforach oraz pamięci współdzielonej.

Kolejne wykonywane zadania:

1. Sprawdza poprawność wprowadzonych argumentów

2. W zależności od przekazanych parametrów przypisuje je odpowiednim zmiennym.

   - Przekazany **1 argument**:

     Odczytanie stanu konta wskazanego przez argument, uruchomienie programu Zad1 poprzez komendę np.

     ```bash
     ./Zad1 0
     ```

     Wymusza działanie programu na odczytanie wartości konta nr 0 (konta numerowane od 0 do liczby określonej jako `#define bankAccounts ` w pliku `SMLibrary.h`).


   - Przekazane **2 argumenty**:

     Zwiększenie lub zmniejszenie o kwotę przekazaną jako drugi argument (wpłata / wypłata) stanu konta o numerze przekazanym jako pierwszy argument , uruchomienie programu Zad1 poprzez komendę np.

     ```bash
     ./Zad1 1 350
     ```

   - Przekazane **3 argumenty**:

     Przelanie kwoty (3 argument) z konta o numerze pierwszego argumentu, na konto o numerze drugiego argumentu. 

     ```bash
     ./Zad1 1 2 350
     ```

3. Utworzenie i zainicjalizowanie grupy semaforów przez pierwszy proces lub dołączenie do utworzonych semaforów przez kolejne procesy

4. Utworzenie i przypisanie pamięci współdzielonej w przypadku pierwszego procesu lub jedynie przypisanie w przypadku kolejnych procesów.

5. Wykonywanie operacji opisanych w kroku 2.

6. Odłączenie pamięci współdzielonej

7. Jeżeli jest to ostatni proces działający na pamięci współdzielonej, skasowanie jej i usunięcie semaforów.

8. Zakończenie działania programu.



Przykładowy Listing programu:

```bash
/media/sf_Lab/Lab4$ ./InitScript 12
    Created Semaphore with ID: 4489219
    Attached to exist semaphore with ID: 4489219
    Created SM with ID: 13139982
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist semaphore with ID: 4489219
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist SM with ID: 13139982
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
    Attached to exist semaphore with ID: 4489219
    Attached to exist SM with ID: 13139982
            Bank balance (account 1) before transaction: 0.000000

            ....Transaction....

            Bank balance (account 1) after transaction: -25.000000
            Bank balance (account 2) before transaction: 0.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 25.000000
            Bank balance (account 1) before transaction: -25.000000

            ....Transaction....

            Bank balance (account 2) before transaction: 25.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 75.000000
            Bank balance (account 1) before transaction: 75.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 175.000000
            Bank balance (account 2) before transaction: 175.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 175.000000
            Bank balance (account 2) after transaction: 325.000000
            Bank balance (account 2) before transaction: 325.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 475.000000
            Bank balance (account 2) before transaction: 475.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 625.000000
            Bank balance (account 1) before transaction: 175.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 150.000000
            Bank balance (account 2) before transaction: 625.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 650.000000
            Bank balance (account 1) before transaction: 150.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 125.000000
            Bank balance (account 2) before transaction: 650.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 675.000000
            Bank balance (account 1) before transaction: 125.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 225.000000
            Bank balance (account 1) before transaction: 225.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 200.000000
            Bank balance (account 2) before transaction: 675.000000

            ....Transaction....

            Bank balance (account 2) after transaction: 700.000000
            Bank balance (account 1) before transaction: 200.000000

            ....Transaction....

            Bank balance (account 1) after transaction: 300.000000
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Shared Memory disconnected!
    Process with PID 9922 exited with status 0x100.
    Process with PID 9923 exited with status 0x100.
    Process with PID 9921 exited with status 0x100.
    Process with PID 9919 exited with status 0x100.
    Process with PID 9917 exited with status 0x100.
    Process with PID 9918 exited with status 0x100.
    Process with PID 9924 exited with status 0x100.
    Process with PID 9916 exited with status 0x100.
    Process with PID 9920 exited with status 0x100.
    Process with PID 9915 exited with status 0x100.
    Shared Memory removed!
    Semafor removed!
    Process with PID 9914 exited with status 0x100.
    Process with PID 9913 exited with status 0x100.
```

