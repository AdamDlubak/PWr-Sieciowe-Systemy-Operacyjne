#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){ 

	int status, p;
  
	switch(p = fork()) {
		/* Sprawdzanie wystąpienia błędu */
		case -1:
			perror("Error fork()"); exit(0);
		break;

		/* Proces potomny */
		case 0:
			printf("Child:\n \tPID:  %d\n\tPPID: %d\n", getpid(), getppid()); 
			exit(0); 
		break;

		/* Proces macierzysty */
		default:
			printf("Parent:\n \tPID:  %d\n\tPPID: %d\n", getpid(), getppid()); 
		break;
	}
	
	/* Kończenie procesów */	 
	p = wait(&status);
	if(p == -1) { perror("Error wait()"); exit(0); }
	exit(0);


	return 1;
} 
