#include "SemLibrary.h"

// ... i zainicjowanie semafora jako podniesiony (S=1)
int bSemCreate(key_t key, char* path, int k, int semAmount) {
    int semId, i;
    union semun setter;
        if ((key = ftok(path, k)) == -1) {
            perror("Error ftok()"); exit(0);  
        } 
            if ((semId = semget(key, semAmount, IPC_CREAT | IPC_EXCL | 0666)) != -1) {
                printf("Created Semaphore with ID: %d\n", semId);
                setter.val = 1;
                for(i = 0; i < semAmount; i++){
                    if (semctl(semId, i, SETVAL, setter) == -1) {
                        fprintf(stderr, "blad ustawienia semafora\\n");
                        exit(1);
                    }
                }
            } else {
                /* If segment exist and error another than i.e. access denied or not enough memory */
                if(errno != EEXIST) {
                    perror("Error semget()"); exit(0); 
                }
                else {
                    if ((semId = semget(key, semAmount, 0600)) == -1) {
                        perror("Error semget() during connecting"); exit(0);   
                    }
                    printf("Attached to exist semaphore with ID: %d\n", semId); 
                }
            }        
            return semId;
}

void bSemBlockP(int semafor) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;
    
    if (semop(semafor, &operation, 1) == -1)
    {
    fprintf(stderr, "blad blokowania semafora\n");
    exit(1);
    }
    
}
void bSemUnblockV(int semafor) {
    struct sembuf operation;
    
    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = 0;
    
    if (semop(semafor, &operation, 1) == -1)
    {
    fprintf(stderr, "blad odblokowywania semafora\n");
    exit(1);
    }
    
    
}
int bSemDelete(int semafor, int semNumber){
       return semctl(semafor, semNumber, IPC_RMID);
}