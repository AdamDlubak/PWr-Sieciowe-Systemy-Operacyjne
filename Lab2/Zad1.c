#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <limits.h>

int main(int argc, char * argv[]){ 

    const int sizeToSend = 24576;
    const int sizeToRead = 4096;
    int status, p, i;
    char messageToSend[sizeToSend];
    char messageToRead[sizeToRead];

    /* Zapisywanie wiadomości */
    for(i = 0; i < sizeToSend; i++){
        messageToSend[i] = 'X';
    }
    int fd[2];
    int iterations = atoi(argv[1]);

    /* Sprawdzanie poprawności podanego argumentu funckji, jeśli błędna - wartość domyślna */
    if(argc != 2 && iterations < 1 ) iterations = 100;
	p = pipe(fd);
	if(p == -1) { perror("Error pipe()"); exit(0); }

	switch(p = fork()) {
		case -1:
			perror("Error fork()"); exit(0);
		break;

        case 0: 	
            for(i = 0; i < iterations; i++){
                close(fd[0]);
                p = write(fd[1], &messageToSend, sizeof(messageToSend));
                if(p == -1) printf("Child:\t(Iter: %d)\tI wrote everything what I could\n", i);
                else printf("Child:\t(Iter: %d)\tI wrote  message\t(size: %d b)\n", i, p);  
            }			
			exit(0); 
		break;

        default:
            for(i = 0; i < iterations; i++){
                sleep(1);
                close(fd[1]); 
                p = read(fd[0], &messageToRead, sizeof(messageToRead)); 
                if(p == 0) printf("Parent:\t(Iter: %d)\tI read everything what I could\n", i);
                else printf("Parent:\t(Iter: %d)\tI read message\t(size: %d b)\n", i, p);
            }
		break;
	}

    /* Kończenie procesów */	 
	p = wait(&status);
	if(p == -1) { perror("Error wait()"); exit(0); }
	exit(0);

	return 1;
} 