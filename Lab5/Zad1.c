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
	struct transferParameters {
		int accountFrom;
		int accountTo;
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
void *makeTransfer(void *transParam) {

		struct transferParameters *param = (struct transferParameters*)transParam;

		/* Entering to Critical Section AccountFrom */
        int result = pthread_mutex_lock(&mutex[(*param).accountFrom]);
		if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 
		while (semAccount[(*param).accountFrom] <= 0 || (bankBalance[(*param).accountFrom] < (*param).value)) {
			if(bankBalance[(*param).accountFrom] >= (*param).value) { /* Jeżeli są środki na koncie, lecz konta zajęte, czekaj */
					pthread_cond_wait(&condWithDrawal[(*param).accountFrom], &mutex[(*param).accountFrom]);
					continue;
			} else {
				if(condWithDrawalPriorBusy[(*param).accountFrom]) { /* Jeśli kolejka priorytetowa zajęta, to w zwykłej */
					pthread_cond_wait(&condWithDrawal[(*param).accountFrom], &mutex[(*param).accountFrom]);
				} else {  /* Jeśli kolejka priorytetowa wolna, to w niej */	
					condWithDrawalPriorBusy[(*param).accountFrom]++;
					printf("Thread: %d\tAccount: %d\tPriority cond was sent to sleep!\n", (*param).threadNumber, (*param).accountFrom);
					
					pthread_cond_wait(&condWithDrawalPrior[(*param).accountFrom], &mutex[(*param).accountFrom]);
					printf("Thread: %d\tAccount: %d\tPriority cond was awake!\n", (*param).threadNumber, (*param).accountFrom);
					condWithDrawalPriorBusy[(*param).accountFrom]--;	
				}
			}
		}
		semAccount[(*param).accountFrom]--;
		result = pthread_mutex_lock(&mutex[(*param).accountTo]);
		if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 
		while (semAccount[(*param).accountTo] <= 0) {
			pthread_cond_wait(&condDeposit[(*param).accountTo], &mutex[(*param).accountTo]);
		}
		semAccount[(*param).accountTo]--;

		result = pthread_mutex_unlock(&mutex[(*param).accountFrom]);
			if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); }
		result = pthread_mutex_unlock(&mutex[(*param).accountTo]);
			if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); } 



/* .................... Działania .................... */
			int tmpFrom = bankBalance[(*param).accountFrom];
			int tmpTo = bankBalance[(*param).accountTo];
			tmpFrom -= (*param).value;
			tmpTo += (*param).value;
			sleep(1);
			bankBalance[(*param).accountFrom] = tmpFrom;
			bankBalance[(*param).accountTo] = tmpTo;
			printf("Thread: %d\tAccount: %d\tBank balance after transfer from me  %d$: %d$\n", (*param).threadNumber, (*param).accountFrom, (*param).value, bankBalance[(*param).accountFrom]);
			printf("Thread: %d\tAccount: %d\tBank balance after transfer to me %d$: %d$\n", (*param).threadNumber, (*param).accountTo, (*param).value, bankBalance[(*param).accountTo]);



		/* Escaping from Critical Section */
        result = pthread_mutex_lock(&mutex[(*param).accountFrom]);
			if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 

        result = pthread_mutex_lock(&mutex[(*param).accountTo]);
			if (result != 0) { perror("Error pthread_mutex_lock\n"); exit(0); } 
		semAccount[(*param).accountFrom]++;
		semAccount[(*param).accountTo]++;



		if(condWithDrawalPriorBusy[(*param).accountFrom]) {
			printf("Thread: %d\tAccount: %d\tI awake priority cond!\n", (*param).threadNumber, (*param).accountFrom);
			pthread_cond_signal(&condWithDrawalPrior[(*param).accountFrom]);
		}
		if(condWithDrawalPriorBusy[(*param).accountTo]) {
			printf("Thread: %d\tAccount: %d\tI awake priority cond!\n", (*param).threadNumber, (*param).accountTo);
			pthread_cond_signal(&condWithDrawalPrior[(*param).accountTo]);			
		}
		pthread_cond_signal(&condWithDrawal[(*param).accountFrom]);		
		pthread_cond_signal(&condWithDrawal[(*param).accountTo]);		
		pthread_cond_signal(&condDeposit[(*param).accountFrom]);
		pthread_cond_signal(&condDeposit[(*param).accountTo]);

	
	result = pthread_mutex_unlock(&mutex[(*param).accountFrom]);
		if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); } 
	result = pthread_mutex_unlock(&mutex[(*param).accountTo]);
		if (result != 0) { perror("Error pthread_mutex_unlock\n"); exit(0); } 

		
	   free(transParam);
		
	   return NULL;
	   
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
		sleep(1);
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
		if(awakeDeposit[(*param).account]) {
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
	struct transferParameters *transParam;
    int i, result, threadsAmt = 0;
	int threadsArray[4] = { 1, 2, 1, 6}; 										/* Number of particular operations */
	int values[10] = { 100, 50, -50, 150, -50, -50, 1000, -50, 10 , 50};		/* Values of deposits */	
	for(i = 0; i < 4; i++) { threadsAmt += threadsArray[i]; }
    pthread_t bankThread[threadsAmt]; 											/* General number of threads */

	for(i = 0; i < threadsAmt; i++) {
		sleep(1);
		if(i%2 == 0) {
			/* Preparing data */
			depParam = malloc(sizeof(struct depositParameters));
			(*depParam).account = randomInt(0, 2);
			(*depParam).value = values[i];
			(*depParam).threadNumber = i;

			/* Creating threads */
			result = pthread_create(&bankThread[i], NULL, makeDeposit, (void*) depParam);
			if (result != 0) {
				perror("Error pthread_create() - Could not create thread!\n");
			}
		}
		else {
			transParam = malloc(sizeof(struct transferParameters));
			(*transParam).accountFrom = 1;
			(*transParam).accountTo = 0;
			(*transParam).value = abs(values[i]);
			(*transParam).threadNumber = i;	

			/* Creating threads */
			result = pthread_create(&bankThread[i], NULL, makeTransfer, (void*) transParam);
			if (result != 0) {
				perror("Error pthread_create() - Could not create thread!\n");
			}
		}



	}

	/* Joining threads */
	for(i = 0; i < threadsAmt; i++) {
		result = pthread_join(bankThread[i], NULL);
	
		if(result != 0) {
			perror("Error pthread_join() - Could not join thread!\n");
		}
	}

    return 1;
}   
