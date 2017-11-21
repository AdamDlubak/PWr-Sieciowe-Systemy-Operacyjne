#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>
#include <sys/wait.h>
#include <strings.h>
#include <string.h>

#define h_addr h_addr_list[0] /* for backward compatibility */


int main(int argc, char * argv[]){ 

	int sockfd, portNumber, n;
    struct sockaddr_in serverAddress;
    struct hostent *server;
    char buffer[256];
    char x;
    if (argc < 3) { perror("Client error: Wrong parameters amount - needed 2 ( hostname port ) \n"); exit(0); }

	server = gethostbyname(argv[1]);
    if (server == NULL) { perror("Client error: No such host - gethostbyname()\n"); exit(0); }
	
    portNumber = atoi(argv[2]);
	

	// Connecting to the server (open socket)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) { perror("Client error: Opening socket - socket()\n"); exit(0); }
	
    bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serverAddress.sin_addr.s_addr,
         server->h_length);
    serverAddress.sin_port = htons(portNumber);
	if (connect(sockfd,(struct sockaddr *)&serverAddress,sizeof(serverAddress)) < 0) 
        perror("Client error: Connecting problem - connect()\n");
	else printf("Connected!\n");

		
			printf("\e[1;1H\e[2J");
            while(1)
            {
                printf("All the time I will waiting for Your command!\n");
                printf("\t1.\tWrite a message\n");
                printf("\t2.\tShow directory\n");
                printf("\t3.\tGet a file\n");
                printf("\n\t9.\tQuit\n");
				x = getchar();
                printf("\e[1;1H\e[2J");
                switch(x)
                {
					case '1': 
						// Clean buffer
						fseek(stdin, 0, SEEK_END);
						// Sending a message
						printf("Please enter the message: ");
						bzero(buffer,256);
						fgets(buffer,255,stdin);
						n = write(sockfd,buffer,strlen(buffer));
						if (n < 0) perror("Client error: Writing to socket - write()\n");
						
						// Waiting for response
						bzero(buffer,256);
						n = read(sockfd,buffer,255);
						if (n < 0) perror("Client error: Reading from socket - read()\n");
						printf("%s\n",buffer);

             		    getchar();
						
                        break;
                    
                    default:
                        printf("Unknown command");
                        break;
                }     
        }
    return 1;
}   
