#include "SHLibrary.h"
#include "SemLibrary.h"
/*
Create or attach Shared memory using key and result by parameter 1 if 
SH was created or and 2 if was attached to existing SM.
Function returns Shared Memory Id.
*/
struct shData* createOrGetSM(int key, int* shmId, int semId) {
    
    struct shData* data;
    int tmp;
    /* Try creating share memory */
    if((*shmId = shmget(key, sizeof(struct shData), 0666 | IPC_CREAT | IPC_EXCL)) != -1) {
        printf("Created SM with ID: %d\n", *shmId);   

        data = shmat(*shmId, 0, 0);
        if(data->clients < 0) {
            perror("Error shmat()"); exit(0);
        } 
        bSemBlockP(semId);
        data->bankBalance = 0;
        data->clients = 1;
        bSemUnblockV(semId);
    } else {
        /* If segment exist and error another than i.e. access denied or not enough memory */
        if(errno != EEXIST) {
        perror("Error shmget()"); exit(0); 
        }
        /* If segment exist and possible to attach */
        else {
            if((*shmId = shmget(key, sizeof(struct shData), 0666 | IPC_CREAT)) == -1) {
                perror("Error shmget()"); exit(0); 
            }
            printf("Attached to exist SM with ID: %d\n", *shmId);

            data = shmat(*shmId, 0, 0);
            if(data->clients < 0) {
                perror("Error shmat()"); exit(0);
            } 
            bSemBlockP(semId);    
            tmp = data->clients;
            tmp++;
            data->clients = tmp;   
            bSemUnblockV(semId);
            
        }
    }
    return data;
}

int disconnectSM(struct shData* data, int semId){
    
    int clients;

    bSemBlockP(semId);   
    clients = data->clients;
    clients--;    
    data->clients = clients;    
    bSemUnblockV(semId);
    
    if (shmdt(data) == -1) {
        perror("Error shmdt()");
        exit(1);
    }   

    printf("Shared Memory disconnected!\n");   
    return clients;
}

void removeSM(int clients, int shmId) { 
    if(clients == 0){
        if(shmctl(shmId, IPC_RMID, 0) == -1) {
            perror("Error shmctl()"); exit(0);
        }
        printf("Shared Memory removed!\n");
    }
}

void checkBalance(struct shData* data){
    printf("\tCurrent bank balance %f\n", data->bankBalance);
}

void makeDeposit(int bankAccount, double deposit, struct shData* data){

        double dTmp;

        /* printf("\tDeposit value: %f\n", deposit);     */
        printf("\tBank balance before transaction: %f\n\n\t....Transaction....\n\n", data->bankBalance);

        dTmp = data->bankBalance;
        dTmp += deposit;
        usleep(randomInt(300000, 3000000)); /* Simulate of realization transaction  */ 
        data->bankBalance = dTmp;

        printf("\tBank balance after transaction: %f\n", data->bankBalance);
         
}

int randomInt(int min, int max) {
    int tmp;
    if (max>=min)
        max-= min;
    else
    {
        tmp= min - max;
        min= max;
        max= tmp;
    }
    return max ? (rand() % max + min) : min;
}