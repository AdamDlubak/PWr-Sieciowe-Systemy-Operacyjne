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

#ifndef _SHLIBRARY_H
#define _SHLIBRARY_H

#define SHM_SIZE 1024  /* 1KB shared memory segment */
#define KEY_VALUE 5142 /* Key value for Shared memory and semaphore */
#define bankAccounts 3 /* Amount of bank accounts */

struct shData {
    double bankBalance[bankAccounts];
    int clients;
};

struct shData* createOrGetSM(int, int*, int);

int disconnectSM(struct shData*, int);

void removeSM(int, int);  

void checkBalance(struct shData*, int);

void makeDeposit(int, double, struct shData*);

int randomInt(int, int);

#endif