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
#include "SHLibrary.h"


int main(int argc, char * argv[]){ 
    
    if(argc == 1) {
        printf("Incorrect parameters quantity!\nProgram will close...\n");
        exit(0);
    }

    key_t key = KEY_VALUE;
    int shmId;
    double deposit;
    struct shData *data;
    int clients, result = -1;

    if(argc == 3) {
        deposit = atof(argv[2]);    
    }

    shmId = createOrGetSM(key, &result);
    data = attachSM(shmId, result);




    if(argc == 3 ) {
        makeDeposit(0, deposit, data);
    }
    else if(argc == 2) {
        checkBalance(data);
    }

    clients = disconnectSM(data);
    removeSM(clients, shmId);    
    
    return 1;
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

