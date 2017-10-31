#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/sem.h>
#include "library.h"

#define SHM_SIZE 1024  /* make it a 1K shared memory segment */

int main(int argc, char * argv[]){ 
    
    int shmid;
    key_t key = 5660;
    double sum;
    double *data;
    double deposit;
    if(argc == 3){
        deposit = atof(argv[2]);    }
    /* shmget(klucz, Rozmiar pamięci współdzielonej, podane niżej flagi powodują błąd EEXIST jeśli segment o podanym kluczu już istnieje) */
    if((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        if(errno != EEXIST) {
        /* Segment jeszcze nie istnieje i wysąpił inny błąd, np. brak praw czy brak pamięci na segment */
        perror("Error shmget()"); exit(0); 
        }
        else {
            if((shmid = shmget(key, SHM_SIZE, IPC_CREAT)) == -1){        perror("Error shmget()"); exit(0); }
            /* Proces  istneieje i uzyskałem do niego klucz */

            printf("Dołączyłem się do pamięci współdzielonej o identyfikatorze: %d\n", shmid);
        }
     } else {
        /* Utworzenie pamięci współdzielonej i ustalenie wartości konta na 0 */
  
        sum = 0;
        printf("Utworzyłem pamięć o identyfikatorze: %d\n", shmid);
        printf("Wartość konta to: %f\n", sum);
    }
    if((data = (double *) shmat(shmid, NULL, 0)) == (double *)-1){
        perror("Error shmat():"); exit(0);
    } 
/*
    if((key = ftok(".", 'A')) == -1) {
        perror("Error ftok()"); exit(0);
    }

    if((semId = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("Error semget()"); exit(0);
    }

*/
    /* Inicjowanie semfora jako podniesiony */
/*    ustaw.val = 1;
    if(semctl(semId, 0, SETVAL, ustaw) == -1){
        perror("Error semctl()"); exit(0);
    }



    if(semop(semId, &operacja, 1) == -1) {
        perror("Error semop()"); exit(0);
    }
*/
    /* Wykonuję jakieś operacje */

    if(argc == 3 ) {
        /* strncpy(data, &deposit, sizeof(double)); */
        printf("Poczatkowa wartosc konta: %f\n", *data);
        
        *data += deposit;
        printf("Deposit value %f\n", deposit);
        
         printf("Writing to segment %f\n", *data);
         
     }
     else {
         printf("segment contains \"%f\"\n", *data);
     }
     
     
     
     printf("\n\n%f\n\n", *data);
     

	return 1;
} 