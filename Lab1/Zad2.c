#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){ 

	int status;
	int fd[2];
	int p, pid, result;

					
	p = pipe(fd);
	if(p == -1) { perror("Error pipe()"); exit(0); }

	switch(p = fork()) {
		case -1:
			perror("Error fork()"); exit(0);
		break;

		case 0: 	
			pid = getpid();
			printf("Child:\t I sent very important message to my parent!\n");  
			write(fd[1], &pid, sizeof(pid));	
		
			close(fd[1]);	
			exit(0); 
		break;

		default:
			close(fd[1]); 
			read(fd[0], &result, sizeof(result)); 
			printf("Parent:\t My child told me PID number: %d\n\n", result);
			close(fd[0]);
		break;
	}
  
	
/* Kończenie procesów */	 
	p = wait(&status);
	if(p == -1) { perror("Error wait()"); exit(0); }
	exit(0);

	return 1;
} 
