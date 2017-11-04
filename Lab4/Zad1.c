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
#include "SemLibrary.h"
#include "SHLibrary.h"


int main(int argc, char * argv[]){ 
    
    if(argc == 1 || argc > 4) {
        printf("Incorrect parameters quantity!\nProgram will close...\n");
        exit(0);
    }

    key_t key = KEY_VALUE;
    int shmId = 0, semId;
    double deposit;
    struct shData *data;
    int clients, account, account2;
    if(argc == 2){
        account = atof(argv[1]);    
    }
    else if(argc == 3) {
        account = atof(argv[1]);    
        deposit = atof(argv[2]);    
    }
    else if(argc == 4) {
        account = atof(argv[1]);    
        account2 = atof(argv[2]);    
        deposit = atof(argv[3]);    
    }
    semId = bSemCreate(key, "/tmp/", 'B', bankAccounts); 
    
    data = createOrGetSM(key, &shmId, semId);
    sleep(3);
   
    if(argc == 2) {
        checkBalance(data, account);
    } 
    else if(argc == 3 ) {
        bSemBlockP(semId, account);
        makeDeposit(account, deposit, data);
        bSemUnblockV(semId, account);
    }
    else if(argc == 4) {
        bSemBlockP(semId, account);
        bSemBlockP(semId, account2);
        makeDeposit(account, (-1) * deposit, data);
        makeDeposit(account2, deposit, data);
        bSemUnblockV(semId, account2);    
        bSemUnblockV(semId, account);    
    }
    sleep(0.5);
    clients = disconnectSM(data, semId);
    

    sleep(5);
    if(clients == 0) {
        removeSM(clients, shmId);        
        bSemDelete(semId, 0);
        printf("Semafor removed!\n");
    }  
    
    return 1;
}   