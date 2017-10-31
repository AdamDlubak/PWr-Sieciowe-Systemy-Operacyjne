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
struct shData {
    double suma;
    int attached;
};
int main(int argc, char * argv[]){ 
    
    key_t key = 5660;    
    int shmid;
    double sum;
    double deposit;
    struct shData *data;
    int tmp;
    double dTmp;


    if(argc == 3){
        deposit = atof(argv[2]);    }
    /* shmget(klucz, Rozmiar pamięci współdzielonej, podane niżej flagi powodują błąd EEXIST jeśli segment o podanym kluczu już istnieje) */
    if((shmid = shmget(key, sizeof(struct shData), 0666 | IPC_CREAT | IPC_EXCL)) == -1) {
        if(errno != EEXIST) {
        /* Segment jeszcze nie istnieje i wysąpił inny błąd, np. brak praw czy brak pamięci na segment */
        perror("Error shmget()"); exit(0); 
        }
        else {
            if((shmid = shmget(key, sizeof(struct shData), 0666 | IPC_CREAT)) == -1){        perror("Error shmget()"); exit(0); }
            /* Proces  istneieje i uzyskałem do niego klucz */

            printf("Dołączyłem się do pamięci współdzielonej o identyfikatorze: %d\n", shmid);
            if((data = shmat(shmid, 0, 0)) == (int*)-1){
                perror("Error shmat():"); exit(0);
            }
            tmp = data->attached;
            tmp++;
            data->attached = tmp;
            printf("Po dolaczeniu wartosc konta: %f\n", data->suma);
            printf("Po dolaczeniu wartosc dolaczen: %d\n", data->attached);        
        }
     } else {
        /* Utworzenie pamięci współdzielonej i ustalenie wartości konta na 0 */
        if((data = shmat(shmid, 0, 0)) == (int*)-1){
            perror("Error shmat():"); exit(0);
        } 
        data->suma = 0;
        data->attached = 1;
        printf("Utworzyłem pamięć o identyfikatorze: %d\n", shmid);
        printf("Poczatkowa wartosc konta: %f\n", data->suma);
        printf("Poczatkowa wartosc dolaczen: %d\n", data->attached);
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
        printf("Poczatkowa wartosc konta: %f\n", data->suma);
        printf("Poczatkowa wartosc dolaczen: %f\n", data->attached);
        dTmp = data->suma;
        dTmp += deposit;
        sleep(5);
        data->suma = dTmp;
        printf("Deposit value %f\n", deposit);
        
         printf("Writing to segment %f\n", data->suma);
         
     }
     else {
         printf("segment contains \"%f\"\n", data->suma);
     }
    printf("Rozpoczynam czekanie na odłączenie pamięci\n");
    sleep(5);
    
    printf("Odłączam pamięć\n");    
    /* detach from the segment: */
    tmp = data->attached;
    tmp--;
    data->attached = tmp;
    if(tmp == 0){
        if (shmdt(data) == -1) {
            perror("shmdt");
            exit(1);
        }   
    printf("Pamięć odłączona\n");
    
        printf("Kasuję pamięć\n");
        sleep(5);
        
        if(shmctl(shmid, IPC_RMID, 0) == -1)
        {        perror("Error shmctl():"); exit(0);
    }
        printf("Pamięć skasowana\n");
    }
    else {
    if (shmdt(data) == -1) {
        perror("shmdt");
        exit(1);
    }
    printf("Pamięć odłączona\n");
    
}
    

	return 1;
} 