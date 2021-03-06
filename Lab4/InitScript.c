#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char * argv[]) { 
    int i, p, clients, status;
    if(argc < 2) {
        printf("Incorrect parameters quantity!\nProgram will close...\n");
        exit(0);
    }
    clients = atoi(argv[1]);    
    
    for(i = 0; i < clients; i++){
        switch(p = fork()) {
            case -1:
                perror("Error fork()"); exit(0);
                break;
            case 0: 
                if(i%4 == 0) {
                    
                    execl("./Zad1", "Zad1", "1", "100", "5", NULL);                
                }
                else if(i%4 == 1) {
                    execl("./Zad1", "Zad1", "2", "150", "12", NULL);                
                } else if(i%4 == 2) {
                    execl("./Zad1", "Zad1", "1", "2", "25", "2", NULL);                                
                } else {
                    if(i%2 == 0) {
                        execl("./Zad1", "Zad1", "1", "1", NULL);                                
                    } else {
                        execl("./Zad1", "Zad1", "2", "1", NULL);                                
                    }
                }
                exit(0); 
                break;        
            default:
                break;
        }
    }
    pid_t pid;
    while (clients > 0) {
      pid = wait(&status);
      printf("Process with PID %ld exited with status 0x%x.\n", (long)pid, status);
      --clients;
    }
    return 1;
}   
