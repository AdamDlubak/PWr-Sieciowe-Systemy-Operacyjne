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

    int fd,r, p, mode, goToSpace, status, result;
    char x;
    mode = 0;
    char* myFifo = "/tmp/musicFifo";
    
    /*Commands*/
    char msg1[] = "pause\n";
    char msg2[] = "quit\n";
    char msg3[] = "mute 0\n";
    char msg4[] = "mute 1\n";
    char msg5[] = "speed_set 1\n";
    char msg6[] = "speed_set 2\n";
    char msg7[] = "speed_set 5\n";
    char msg8[] = "seek -5 0\n";
    char msg9[] = "seek +5 0\n";
    
    unlink(myFifo);
    r= mkfifo(myFifo, 0666);
	if(r < 0) { perror("Error mkfifo()"); exit(0); }

    switch(p = fork()) {
		case -1:
			perror("Error fork()"); exit(0);
		break;

        case 0: 
            goToSpace = open("/dev/null", O_WRONLY);
            result = dup2(goToSpace, 1);
            if(result == -1) { perror("Error dup2(), Descriptor 1"); exit(0); }
            result = dup2(goToSpace, 2);
            if(result == -1) { perror("Error dup2(), Descriptor 2"); exit(0); }
            execl("/usr/bin/mplayer", "mplayer", "-input", "file=/tmp/musicFifo", "Sample.mp3", NULL);	
            exit(0);
		break;

        default:

            fd = open(myFifo, O_WRONLY);
            if(fd < 0) { perror("Error open()"); exit(0); }


            printf("\e[1;1H\e[2J");
            printf("---------- Last Command: --------------------\n\n");
            while(1)
            {
                printf("All the time I will waiting for Your command!\n");
                printf("\tp\t---\tPlay\\Pause\n");
                printf("\t1\t---\tSpeed x1\n");
                printf("\t2\t---\tSpeed x2\n");
                printf("\t5\t---\tSpeed x5\n");
                printf("\t[\t---\tSeek 5 sec back\n");
                printf("\t]\t---\tSeek 5 sec forward\n\n");
                printf("\tq\t---\tQuit\n");
                x = getchar();
                printf("\e[1;1H\e[2J");
                switch(x)
                {
                    case 'p': 
                        if(mode == 0) {
                            printf("---------- Last Command: Pause --------------\n\n");
                            mode = 1;
                        }
                        else {
                            printf("---------- Last Command: Play ---------------\n\n");
                            mode = 0;
                        }
                        write(fd, msg1, strlen(msg1));
                        break;
                    case 'q': 
                    printf("---------- Last Command: Quit... -----------\n\n");
                        write(fd, msg2, strlen(msg2));
                        sleep(2);
                        exit(1);
                        break;
                    case 'm': 
                    printf("---------- Last Command: Mute Off -----------\n\n");
                        write(fd, msg3, strlen(msg3));
                        break;
                    case 'n': 
                    printf("---------- Last Command: Mute On ------------\n\n");
                        write(fd, msg4, strlen(msg4));
                        break;
                    case '1': 
                    printf("---------- Last Command: Speed x1 -----------\n\n");                    
                        write(fd, msg5, strlen(msg5));
                        break;
                    case '2': 
                    printf("---------- Last Command: Speed x2 -----------\n\n");                    
                        write(fd, msg6, strlen(msg6));
                        break;
                    case '5': 
                    printf("---------- Last Command: Speed x5 -----------\n\n");
                        write(fd, msg7, strlen(msg7));
                        break;
                    case '[': 
                        printf("---------- Last Command: Seek 5 sec back ----\n\n");
                        write(fd, msg8, strlen(msg8));
                        break;
                    case ']': 
                        printf("---------- Last Command: Seek 5 sec forward -\n\n");
                        write(fd, msg9, strlen(msg9));
                        break;
                    default:
                        printf("Unknown command");
                        break;
                }     
        }
        close(fd);   
        unlink(myFifo);     
		break;
	}

    /* Kończenie procesów */	 
	p = wait(&status);
	if(p == -1) { perror("Error wait()"); exit(0); }
	exit(0); 
   
    return 0;
} 