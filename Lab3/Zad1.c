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

int main(int argc, char * argv[]){ 

    const int sizeToSend = 16384;
    const int sizeToRead = 8192;
    int status, p, i, r;
    char messageToSend[sizeToSend];
    char messageToRead[sizeToRead];

    /* Zapisywanie wiadomości */
    for(i = 0; i < sizeToSend; i++){
        messageToSend[i] = 'X';
    }
    int fd;

    char* myFifo = "/tmp/myFifo";
    int iterations = atoi(argv[1]);
    
    unlink(myFifo);
    r= mkfifo(myFifo, 0666);
	if(r < 0) { perror("Error mkfifo()"); exit(0); }
    
    /* Sprawdzanie poprawności podanego argumentu funckji, jeśli błędna - wartość domyślna */
    if(argc != 2 && iterations < 1 ) iterations = 100;

    switch(p = fork()) {
		case -1:
			perror("Error fork()"); exit(0);
		break;

        case 0: 
            printf("Child\tOpening to write...\n");
            fd = open(myFifo, O_WRONLY);        
            if(fd < 0) { perror("Error open()"); exit(0); }
            printf("Child\tOpened to write!\n");
            
            for(i = 0; i < iterations; i++){
                p = write(fd, &messageToSend, sizeof(messageToSend));
                if(p == -1) printf("Child:\t(Iter: %d)\tI wrote everything what I could\n", i);
                else printf("Child:\t(Iter: %d)\tI wrote  message\t(size: %d b)\n", i, p);  
            }	
            close(fd);		
			exit(0); 
		break;

        default:
            printf("Parent\tOpening to read...\n");
            fd = open(myFifo, O_RDONLY);
            if(fd < 0) { perror("Error open()"); exit(0); }
            printf("Parent\tOpened to read!\n");
            
            sleep(3);                        
            for(i = 0; i < iterations; i++){
            sleep(1);            
                p = read(fd, &messageToRead, sizeof(messageToRead)); 
                if(p == 0) printf("Parent:\t(Iter: %d)\tI read everything what I could\n", i);
                else printf("Parent:\t(Iter: %d)\tI read message\t(size: %d b)\n", i, p);
            }
            close(fd);
		break;
	}
    /* Kończenie procesów */	 
	p = wait(&status);
    if(p == -1) { perror("Error wait()"); exit(0); }
    close(fd);		    
    unlink(myFifo);    
	exit(0);

	return 1;
} 