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
int main(int argc, char * argv[]){ 
#define h_addr h_addr_list[0] /* for backward compatibility */
int sockfd, portNumber, n;

    struct sockaddr_in serverAddress;
    struct hostent *server;
    char buffer[256];

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

    return 1;
}   
