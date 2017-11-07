  #include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/sem.h>
#include <pthread.h>

	int bankBalance[] = { 0, 0 };
	int semAccount[2] = { 1, 1 };
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond   = PTHREAD_COND_INITIALIZER;

	struct depositParameters {
		int account;
		int value;
	};
int randomInt(int min, int max) {
    int tmp;
    if (max>=min) max-= min;
    else {
        tmp= min - max;
        min= max;
        max= tmp;
    }
    return max ? (rand() % max + min) : min;
}

void *makeDeposit(void *depParam) {

		struct depositParameters *param = (struct depositParameters*)depParam;

		/* Entering to Critical Section */
        int result = pthread_mutex_lock(&mutex);
        	if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 

		while (semAccount[(*param).account] <= 0) {
			pthread_cond_wait(&cond, &mutex);
		}
		semAccount[(*param).account]--;

		result = pthread_mutex_unlock(&mutex);
        	if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); } 

		
		/* Critical Section operation */

		int tmp = bankBalance[(*param).account];
		tmp += (*param).value;
		sleep(1);
		bankBalance[(*param).account] = tmp;
		printf("Wynik mojej operacji (konto %d) po wplacie %d wynosi %d\n", (*param).account, (*param).value, bankBalance[(*param).account]);


		/* Escaping from Critical Section */
        result = pthread_mutex_lock(&mutex);
        	if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 
		semAccount[(*param).account]++;
		pthread_cond_signal(&cond);
		
		result = pthread_mutex_unlock(&mutex);
        	if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); } 
   		free(depParam);   
		return NULL;
}

/* 
	Operation 0: make deposit to account 1
  	Operation 1: make deposit to account 2
 	Operation 2: make transfer from account 1 to account 2
  	Operation 3: make transfer from account 2 to account 1
*/
int main(int argc, char * argv[]){ 

	/* Running parameters */
    struct depositParameters *depParam;
    int i, result, threadsAmt = 0;
	int threadsArray[4] = { 3, 2, 1, 2}; 						/* Number of particular operations */
	int values[6] = { 100, 150, 200, 500, 130, 250 };			/* Values of deposits */	
	for(i = 0; i < 4; i++) { threadsAmt += threadsArray[i]; }
    pthread_t bankThread[threadsAmt]; 							/* General number of threads */
	

	for(i = 0; i < threadsAmt; i++) {

		/* Preparing datas */
		depParam = malloc(sizeof(struct depositParameters));
		(*depParam).account = randomInt(0, 2);
		(*depParam).value = values[randomInt(0, 6)];

		/* Creating threads */
		result = pthread_create(&bankThread[i], NULL, makeDeposit, (void*) depParam);
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
	}

    return 1;
}   
