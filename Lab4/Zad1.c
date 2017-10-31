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

#define SHM_SIZE 1024



int main(int argc, char * argv[]){ 
    int wartosc = 0;
    int shmid;
    char *data;
    int semId; /* Identyfikator semafora */


    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short int* array;
        struct seminfo *__buf;
    } ustaw;

struct sembuf operacja;

    key_t key = 5678;
    /* shmget(klucz, Rozmiar pamięci współdzielonej, podane niżej powodują błąd EEXIST jeśli segment o podanym kluczu już istnieje) */
    if((shmid = shmget(key, sizeof(int), 0644 | IPC_CREAT)) == -1) {
        /* Segment jeszcze nie istnieje i wysąpił inny błąd, np. brak praw czy brak pamięci na segment */
        perror("Error shmget()"); exit(0);    
    }
    /* Proces  istneieje i uzyskałem do niego klucz */
    printf("Utworzyłem pamięć o identyfikatorze: %d\n", shmid);
    if((data = shmat(shmid, NULL, SHM_RND)) == (char*) -1){
        perror("Error shmat():"); exit(0);
    }


    if((key = ftok(".", 'A')) == -1) {
        perror("Error ftok()"); exit(0);
    }

    if((semId = semget(key, 1, IPC_CREAT | IPC_EXCL | 0600)) == -1) {
        perror("Error semget()"); exit(0);
    }


    /* Inicjowanie semfora jako podniesiony */
    ustaw.val = 1;
    if(semctl(semId, 0, SETVAL, ustaw) == -1){
        perror("Error semctl()"); exit(0);
    }




    /* Zablokowanie dostępu do pliku - operacja p */
    operacja.sem_num = 0;
    operacja.sem_op = -1; /* Zablokuj */
    operacja.sem_flag = 0; /* operacja blokująca */

    if(semop(semId, &operacja, 1) == -1) {
        perror("Error semop()"); exit(0);
    }

    /* Wykonuję jakieś operacje */
if(argc == 2) {
    strncpy(data, argv[1], sizeof(int));
    printf("Writing to segment \"%s\"\n", argv[1]);
    
}
else {
    printf("segment contains \"%s\"\n", data);
}




    /* Odblokowanie dostępu */
    operacja.sem_num = 0;
    operacja.sem_op = 1; /* Zablokuj */
    operacja.sem_flag = 0; /* operacja blokująca */

    if(semop(semId, &operacja, 1) == -1) {
        perror("Error semop()"); exit(0);
    }  



    /* Odłączenie od segmentu */
    if(shmdt(data) == -1) {
        perror("Error shmdt()"); exit(0);
    }
    /* Kasowanie pamięci współdzielonej */
    shmctl(key, IPC_RMID, NULL);
    printf("Pamięć współdzielona została zamknięta\n");
    

	return 1;
} 