#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc, char * argv[]){ 

	int status;
	int fd[2];
	int p, result;

	p = pipe(fd);
	if(p == -1) { perror("Error pipe()"); exit(0); }

	switch(p = fork()) {
		case -1:
			perror("Error fork()"); exit(0);
		break;

		case 0:
			/* Przekazanie deskryptora zapisu do pipe'a */
			result = dup2(fd[1], 1);
			if(result == -1) { perror("Error dup2()"); exit(0); }

      		execl("/bin/ls", "ls", "-l", NULL);
			exit(0); 
		break;

		default:
			close(fd[1]); 
			result = dup2(fd[0], 0);
			if(result == -1) { perror("Error dup2()"); exit(0); }
		   	execl("/bin/cat", "cat", "-n", NULL);
			close(fd[0]);
		break;
	}
  
/* Kończenie procesów */	 
	p = wait(&status);
	if(p == -1) { perror("Error wait()"); exit(0); }
	exit(0);

	return 1;
} 
