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
#include <pthread.h>


	int bankBalance[] = { 0, 0 };
	int semAccount[2] = { 1, 1 };
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

void *makeDeposit(void *deposit) {
        int result2 = pthread_mutex_lock(&mutex);
        if (result2 != 0) {
            fprintf(stderr, "Error occured during locking the mutex.\n");
            exit (-1);
        } 
		while (semAccount[0] <= 0) {
			pthread_cond_wait(&cond, &mutex);
		}
		semAccount[0]--;
		result2 = pthread_mutex_unlock(&mutex);
        if (result2 != 0) {
            fprintf(stderr, "Error occured during unlocking the mutex.\n");
            exit (-1);
        }


		int dep = *((int *)deposit);
		int tmp = bankBalance[0];
		tmp += dep;
		sleep(2);
		bankBalance[0] = tmp;
		printf("Wynik mojej operacji %d\n", bankBalance[0]);




        result2 = pthread_mutex_lock(&mutex);
        if (result2 != 0) {
            fprintf(stderr, "Error occured during locking the mutex.\n");
            exit (-1);
        } 
		
		semAccount[0]++;

		pthread_cond_signal(&cond);
		result2 = pthread_mutex_unlock(&mutex);
        if (result2 != 0) {
            fprintf(stderr, "Error occured during unlocking the mutex.\n");
            exit (-1);
        }




	
        
			int *result = malloc(sizeof(int));
			*result = 1;
			return result;

}

/* 
	Operation 0: make deposit to account 1
  	Operation 1: make deposit to account 2
 	Operation 2: make transfer from account 1 to account 2
  	Operation 3: make transfer from account 2 to account 1
*/
int main(int argc, char * argv[]){ 

	/* Running parameters */
    int i, result, threadsAmt = 0;
	int threadsArray[] = { 3, 2, 1, 2}; 						/* Number of particular operations */
	int values[] = { 100, 100, 200, 500, 100, 250 };			/* Values of deposits */	
	for(i = 0; i < 4; i++) { threadsAmt += threadsArray[i]; }
    pthread_t bankThread[threadsAmt]; 							/* General number of threads */
	

	for(i = 0; i < threadsAmt; i++) {
		/* Creating threads */
		result = pthread_create(&bankThread[i], NULL, makeDeposit, &values[0]);
		if (result != 0) {
			perror("Error pthread_create() - Could not create thread!\n");
		}
	}

	/* Joining threads */
	for(i = 0; i < threadsAmt; i++) {
    	int *bankThreadResult;
    	result = pthread_join(bankThread[i], (void **) &bankThreadResult);
		if( result != 0) {
			perror("Error pthread_join() - Could not join thread!\n");
		}
		else {
			printf("Thread returned, %d.\n", *bankThreadResult);
		}
	}

    return 1;
}   
