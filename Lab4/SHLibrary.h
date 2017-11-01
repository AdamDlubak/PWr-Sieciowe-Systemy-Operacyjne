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
#define KEY_VALUE 5660 

struct shData {
    double bankBalance;
    int clients;
};

int createOrGetSM(int, int*);

struct shData* attachSM(int shmId, int result);

int disconnectSM(struct shData*);

void removeSM(int, int);  

void checkBalance(struct shData*);

void makeDeposit(int, double, struct shData*);

#endif