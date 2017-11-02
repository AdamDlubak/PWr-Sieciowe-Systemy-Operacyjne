#include "SHLibrary.h"
#include "SemLibrary.h"
/*
Create or attach Shared memory using key and result by parameter 1 if 
SH was created or and 2 if was attached to existing SM.
Function returns Shared Memory Id.
*/
int createOrGetSM(int key, int* result) {
    
    int shmId;

    /* Try creating share memory */
    if((shmId = shmget(key, sizeof(struct shData), 0666 | IPC_CREAT | IPC_EXCL)) != -1) {
        printf("Created SM with ID: %d\n", shmId);
        *result = 1;        
        return shmId;
    } else {
        /* If segment exist and error another than i.e. access denied or not enough memory */
        if(errno != EEXIST) {
        perror("Error shmget()"); exit(0); 
        }
        /* If segment exist and possible to attach */
        else {
            if((shmId = shmget(key, sizeof(struct shData), 0666 | IPC_CREAT)) == -1) {
                perror("Error shmget()"); exit(0); 
            }
            printf("Attached to exist SM with ID: %d\n", shmId);
            *result = 2;
            return shmId;
        }
    } 
}

struct shData* attachSM(int shmId, int semId, int result) {
    
    struct shData* data;
    int tmp;

    data = shmat(shmId, 0, 0);
    if(data->clients == -1) {
        perror("Error shmat()"); exit(0);
    }
    bSemBlockP(semId);
    /* SM was already created, set basic parameter */
    if(result == 1){
        data->bankBalance = 0;
        data->clients = 1;
    }
    /* Attach to exist SM, increment amount of SM user */
     else if(result == 2) {
        tmp = data->clients;
        tmp++;
        data->clients = tmp;   
    }
    else {
        perror("Result of function createOrGetSM() was incorrect\n");
        exit(0);
    }
    bSemUnblockV(semId);
    
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

        printf("\tDeposit value: %f\n", deposit);    
        printf("\tBank balance before transaction: %f\n\n\t....Transaction....\n\n", data->bankBalance);

        dTmp = data->bankBalance;
        dTmp += deposit;
        sleep(1); /* Simulate of realization transaction  */ 
        data->bankBalance = dTmp;

        printf("\tBank balance after transaction: %f\n", data->bankBalance);
         
}