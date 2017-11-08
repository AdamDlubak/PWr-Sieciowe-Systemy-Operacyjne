# Laboratorium 3

Stworzone w języku c z użyciem łącz nazwanych.

#### Laboratorium nr 3 - 24.10.2017r.



## Makefile

Wykonuje kompilację wszystkich 2 zadań oraz spakowanie ich do archiwum `.zip`. 

**Wywołanie poprzez:**  

```bash
make all
```



## Zadanie 1

Zadanie jest odpowiednikiem zadania pierwszego z poprzedniego laboratorium (Lab2). Różni się ono jedynie sposobem transmisji danych - wykorzystywane jest **łącze nazwane** tworzone poprzez funkcję `mkpipe()`. Wyniki można zaobserowować identyczne jak w przypadku łącza nienazwanego. Wszystko
dokładnie zobrazowano na listingu poniżej.

> W ten sposób utworzony strumień danych (za pomocą łącza nazwanego), może egzystować dłużej niż samo działanie programu (w przeciwieństwie do łącza nienazwanego).
> Pojemnośc takiego strumienia to dokładnie 64 kb.

**Wywołanie poprzez:**  

```bash
./Zad1 ilość-iteracji
```



**Przykładowy listing programu:**

```bash
./Zad1 20
        Parent	Opening to read...
        Child	Opening to write...
        Parent	Opened to read!
        Child	Opened to write!
        Child:	(Iter: 0)	I wrote  message	(size: 16384 b)
        Child:	(Iter: 1)	I wrote  message	(size: 16384 b)
        Child:	(Iter: 2)	I wrote  message	(size: 16384 b)
        Child:	(Iter: 3)	I wrote  message	(size: 16384 b)
        Parent:	(Iter: 0)	I read message	(size: 8192 b)
        Parent:	(Iter: 1)	I read message	(size: 8192 b)
        Child:	(Iter: 4)	I wrote  message	(size: 16384 b)
        Parent:	(Iter: 2)	I read message	(size: 8192 b)
        Parent:	(Iter: 3)	I read message	(size: 8192 b)
        Child:	(Iter: 5)	I wrote  message	(size: 16384 b)
        Parent:	(Iter: 4)	I read message	(size: 8192 b)
        Parent:	(Iter: 5)	I read message	(size: 8192 b)
        Child:	(Iter: 6)	I wrote  message	(size: 16384 b)
        Parent:	(Iter: 6)	I read message	(size: 8192 b)
        Parent:	(Iter: 7)	I read message	(size: 8192 b)
        Child:	(Iter: 7)	I wrote  message	(size: 16384 b)
        Parent:	(Iter: 8)	I read message	(size: 8192 b)
        Parent:	(Iter: 9)	I read message	(size: 8192 b)
        Child:	(Iter: 8)	I wrote  message	(size: 16384 b)
        Parent:	(Iter: 10)	I read message	(size: 8192 b)
        ...(dalsza część listingu została pominięta)...
```



## Zadanie 2

Program poprzez funkcję `fork()` tworzy swojego potomka, który uruchamia program `mplayer` z parametrem `-input` do wcześniej otwartego przez proces rodzica **strumienia nazwanego**. Umożliwia to przejęcie sterowania nad mplayerem.

Sterowanie odbywa się poprzez proste i intuicyjne menu przedstawione poniżej.
Zostały tam zaimplementowane jedynie podstawowe funkcje takie jak *przerwa*, *start*, *przyspieszenie* czy *przesunięcie* utworu w przód lub tył.

Istnieje oczywiście możliwość w bardzy prosty sposób rozbudowania programu o dodatkowe funkcjonalności, np. spowalnianie czy zapętlanie, jednakże nie to było przedmiotem ćwiczeń laboratoryjnych.

**Wywołanie poprzez:**  

```bash
./Zad2
```



**Przykładowy listing programu:**

```bash
./Zad2 
---------- Last Command: Seek 5 sec back ----

All the time I will waiting for Your command!
        p	  ---	    Play\Pause
        1	  ---	    Speed x1
        2	  ---	    Speed x2
        [     ---   	Seek 5 sec back
        ]	  ---	    Seek 5 sec forward

        q     ---    	Quit
```



