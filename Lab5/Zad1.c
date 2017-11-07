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
	pthread_mutex_t mutex[2] = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t condDeposit[2] = PTHREAD_COND_INITIALIZER;
	pthread_cond_t condWithDrawal[2] = PTHREAD_COND_INITIALIZER;
	pthread_cond_t condWithDrawalPrior[2] = PTHREAD_COND_INITIALIZER;
	int condWithDrawalPriorBusy[2] = { 0, 0 };
	int awakeDeposit[2] = { 0, 0 }; /* if awakeDeposit = 1 - before awake condDeposit, if 1 - before awake condWithDrawal */
	struct depositParameters {
		int account;
		int value;
		int threadNumber;
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
		int isWithDrawal;
		struct depositParameters *param = (struct depositParameters*)depParam;
		if((*param).value < 0){
			isWithDrawal = 1;
			awakeDeposit[(*param).account] = 1;
		} else awakeDeposit[(*param).account] = 0;


		/* Entering to Critical Section */
        int result = pthread_mutex_lock(&mutex[(*param).account]);
        	if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 

			if(isWithDrawal && condWithDrawalPriorBusy[(*param).account]){
				pthread_cond_wait(&condWithDrawal[(*param).account], &mutex[(*param).account]);				
			}
		/* Jeżeli semafor zajęty LUB jest to wypłata i brak środków */
		while (semAccount[(*param).account] <= 0 || ((bankBalance[(*param).account] + (*param).value < 0) && isWithDrawal)) {

			if(semAccount[(*param).account] <= 0) { /* Jeżeli tylko semafor zajęty, czekaj w kolejce bez priorytu*/
				if(isWithDrawal) pthread_cond_wait(&condWithDrawal[(*param).account], &mutex[(*param).account]);
				else pthread_cond_wait(&condDeposit[(*param).account], &mutex[(*param).account]);
				continue;				
			} else if(bankBalance[(*param).account] + (*param).value < 0) { /* Jeżeli brak środków do wypłaty, czekaj w kolejce wypłat*/
				if(condWithDrawalPriorBusy[(*param).account]) { /* Jeśli kolejka priorytetowa zajęta, to w zwykłej */
					pthread_cond_wait(&condWithDrawal[(*param).account], &mutex[(*param).account]);
				} else {  /* Jeśli kolejka priorytetowa wolna, to w niej */	
					condWithDrawalPriorBusy[(*param).account]++;
					printf("Thread: %d\tAccount: %d\tPriority cond was sent to sleep!\n", (*param).threadNumber, (*param).account);
					
					pthread_cond_wait(&condWithDrawalPrior[(*param).account], &mutex[(*param).account]);
					printf("Thread: %d\tAccount: %d\tPriority cond was awake!\n", (*param).threadNumber, (*param).account);
					condWithDrawalPriorBusy[(*param).account]--;	
				}		
			}	
		}
		
		semAccount[(*param).account]--;

		result = pthread_mutex_unlock(&mutex[(*param).account]);
        	if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); } 

		
		/* Critical Section operation */

		int tmp = bankBalance[(*param).account];
		tmp += (*param).value;
		sleep(5);
		bankBalance[(*param).account] = tmp;
		if(isWithDrawal) printf("Thread: %d\tAccount: %d\tBank balance after withdrawal %d$: %d$\n", (*param).threadNumber, (*param).account, (*param).value, bankBalance[(*param).account]);
		else printf("Thread: %d\tAccount: %d\tBank balance after deposit %d$: %d$\n", (*param).threadNumber, (*param).account, (*param).value, bankBalance[(*param).account]);


		/* Escaping from Critical Section */
        result = pthread_mutex_lock(&mutex[(*param).account]);
        	if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 
		semAccount[(*param).account]++;

		if(condWithDrawalPriorBusy[(*param).account]) {
			printf("Thread: %d\tAccount: %d\tI awake priority cond!\n", (*param).threadNumber, (*param).account);
			pthread_cond_signal(&condWithDrawalPrior[(*param).account]);
		}
		else if(awakeDeposit[(*param).account]) {
			pthread_cond_signal(&condDeposit[(*param).account]);
			pthread_cond_signal(&condWithDrawal[(*param).account]);	
		} else {
			pthread_cond_signal(&condWithDrawal[(*param).account]);
			pthread_cond_signal(&condDeposit[(*param).account]);	
		}
		
		result = pthread_mutex_unlock(&mutex[(*param).account]);
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
	int threadsArray[4] = { 1, 2, 1, 16}; 	/* Number of particular operations */
	int values[20] = { 100, 100, -1150, -1150, -50, -50, 150, 150, -50, -50, -50, -50, -50, -50, 1000, 1000, -50, -50, 150, 150 };			/* Values of deposits */	
	for(i = 0; i < 4; i++) { threadsAmt += threadsArray[i]; }
    pthread_t bankThread[threadsAmt]; 		/* General number of threads */
	

	for(i = 0; i < threadsAmt; i++) {

		/* Preparing data */
		depParam = malloc(sizeof(struct depositParameters));
		(*depParam).account = i%2;
		(*depParam).value = values[i];
		(*depParam).threadNumber = i;
		if(i%2 == 0) sleep(2);
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
		if(result != 0) {
			perror("Error pthread_join() - Could not join thread!\n");
		}
	}

    return 1;
}   
