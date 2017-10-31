#include "library.h"

int bSemCreate(key_t key, char* path, int k, int semAmount) {
    int semId, i;
    union semun setter;
        if ((key = ftok(path, k)) == -1) {
            perror("Error ftok()"); exit(0);  
        } 
        if ((semId = semget(key, semAmount, IPC_CREAT | 0600)) == -1) {
            perror("Error semget()"); exit(0);   
        }
        setter.val = 1;
        for(i = 0; i < semAmount; i++){
            semctl(semId, i, SETVAL, setter);
        }
        return semId;
}


int bSemDelete(int s, int semNumber){
       return semctl(s, semNumber, IPC_RMID);
   }