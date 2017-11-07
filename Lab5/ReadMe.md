# Symulacja systemu bankowego

Stworzone w języku c z użyciem semaforów i muteksów.

#### Laboratorium nr 5 - 07.11.2017r.



## Makefile

Wykonuje kompilację wszystkich zadań. 

Wywołanie poprzez:  

```bash
make all
```

## Zad1

Program rozwiązuje następujące problemy:

- Wykonuje operacje wpłat i wypłat na dowolnie wylosowane konto.
- Dba o brak możliwości wystąpienia debetu na koncie.
- Daje możliwość wykonywania przelewów między kontami w obu kierunkach.	



Program uruchamia się bezparametrowo. Dostępne parametry można edytować w kodzie źródłowym funkcji `main` pliku`Zad1`:

```c
int values[threadsAmt] = { 100, 50, -50, 150 };	/* Values of deposits or transfers (as abs()) */
int oneOfIsTransfer = 5;					  /* Every fifth operation is a transfer */		
```

Ponieważ tablica wartości kwot, którymi operuje program, a także tablica wątków nie są tablicami dynamicznymi (każda kwota rozliczana jest w osobnym wątku), przy edycji wielkości tablicy `values` konieczna jest zmiana zdefiniowanej stałej `threadsAmt` w 11 linii pliku `Zad1.c`:

```c
#define threadsAmt 4 /* General number of threads */
```



Sprawdzając poprawność programu **należy pamiętać**, iż poprzez losowość dobierania parametrów, możliwe jest wystąpienie sytuacji, iż program nie zakończy się prawidłowo samodzielnie, ponieważ niektóre operacje nie mogą zostać zrealizowane, np.:

- Wypłata środków z konta.
- Przelew środków z jednego konta na drugie.

Powodem wystąpienia tego problemu jest przypadkowe dobranie parametrów w ten sposób, iż program chcąc wypłacić z konta środki zastaje sytuację wystąpienia debetu. Wówczas się usypia i oczekuje na zwiększenie środków na koncie, niestety program zakończył już wykonywanie wszystkich wpłat i przelewów, a więc nigdy nie następuje odpowiednie zwiększenie ilości środków na koncie, tak więc program oczekuje w nieskończoność. Należy zaznaczyć, iż **nie jest to spowodowane zagłodzeniem wątku**, które mogłoby wynikać z ciągłego pomijania wątku ("wyprzedzenia" przez inne działające wątki) i nie wykonania swojego zadania nigdy. Tutaj sytuacja taka nie występuje, operacje, które chcą wykonać swoje zadanie, jednak z powodu braku środków nie mogą tego uczyć, trafiają do kolejki priorytetowej, gdzie w pierwszej kolejności po zwiększeniu salda konta, mają możliwość wykonania swojej akcji.



**Wywołanie poprzez:**  

```bash
./Zad1
```



**Przykładowy Listing programu:**

```c
	./Zad1
Thread: 0       Account: 1      Bank balance after deposit 100$: 100$
Thread: 1       Account: 1      Bank balance after transfer from me  50$: 50$
Thread: 1       Account: 0      Bank balance after transfer to me 50$: 50$
Thread: 2       Account: 0      Bank balance after withdrawal -50$: 0$
Thread: 3       Account: 1      Priority cond was sent to sleep!
Thread: 5       Account: 1      Bank balance after transfer from me  50$: 0$
Thread: 5       Account: 0      Bank balance after transfer to me 50$: 50$
Thread: 5       Account: 1      I awake priority cond!
Thread: 3       Account: 1      Priority cond was awake!
Thread: 3       Account: 1      Priority cond was sent to sleep!
Thread: 6       Account: 1      Bank balance after deposit 1000$: 1000$
Thread: 6       Account: 1      I awake priority cond!
Thread: 3       Account: 1      Priority cond was awake!
Thread: 3       Account: 1      Bank balance after transfer from me  150$: 850$
Thread: 3       Account: 0      Bank balance after transfer to me 150$: 200$
Thread: 7       Account: 1      Bank balance after transfer from me  50$: 800$
Thread: 7       Account: 0      Bank balance after transfer to me 50$: 250$
Thread: 4       Account: 1      Bank balance after withdrawal -50$: 750$
Thread: 8       Account: 1      Bank balance after deposit 10$: 760$
Thread: 9       Account: 1      Bank balance after transfer from me  50$: 710$
Thread: 9       Account: 0      Bank balance after transfer to me 50$: 300$	


```

