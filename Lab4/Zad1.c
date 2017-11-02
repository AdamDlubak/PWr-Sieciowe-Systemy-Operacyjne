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
    
    if(argc == 1) {
        printf("Incorrect parameters quantity!\nProgram will close...\n");
        exit(0);
    }

    key_t key = KEY_VALUE;
    int shmId, semId;
    double deposit;
    struct shData *data;
    int clients, result = -1;

    if(argc == 3) {
        deposit = atof(argv[2]);    
    }
    
    shmId = createOrGetSM(key, &result);
    semId = bSemCreate(key, ".", 'A', 1, result); 
    
    data = attachSM(shmId, semId, result);
    
   if(argc == 3 ) {
        bSemBlockP(semId);
        makeDeposit(0, deposit, data);
        bSemUnblockV(semId);
    }
    else if(argc == 2) {
        checkBalance(data);
    }
    
    clients = disconnectSM(data, semId);
    

    sleep(5);
    if(clients == 0) {
        removeSM(clients, shmId);        
        bSemDelete(semId, 0);
        printf("Semafor removed!\n");
    }  
    
    return 1;
}   