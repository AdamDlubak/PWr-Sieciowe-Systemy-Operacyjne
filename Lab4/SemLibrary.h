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
#include <time.h>

#ifndef _SEMLIBRARY_H
#define _SEMLIBRARY_H

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
    /* Defined in sys/sem.h */
#else
    union semun{
        int val;                        /* vaule for SETVAL */
        struct semid_ds *buf;           /* buffer for IPC_STAT, IPC_SET */
        unsigned short int *array;      /* array for GETALL, SETALL */
        struct seminfo *__buf;          /* buffer for IPC_INFO */
    };
#endif

/*  Create binary semafor with file full permission.
    If semafor exist, get access.
    Return: Semafor Id and init as S = 1
    If error return -1 */
int bSemCreate(key_t, char*, int, int);

/*  Block operation */
void bSemBlockP(int, int);

/*  Unblock operation */
void bSemUnblockV(int, int);

/* Remove semafor */
int bSemDelete(int, int);

#endif