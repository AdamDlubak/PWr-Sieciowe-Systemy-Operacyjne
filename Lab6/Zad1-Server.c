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

	int sockfd, newsockfd, portNumber, clientAddressLength;
	char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;
	int n;

	if (argc < 2) { perror("Server error: Wrong parameters amount - needed 1 ( port )\n"); exit(0); }
    portNumber = atoi(argv[1]);

	// Connecting to the server (open socket)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) { perror("Server error: Opening socket - socket()\n"); exit(0); }
	
	// Convert server address
	bzero((char *) &serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portNumber);
	 
    if (bind(sockfd, (struct sockaddr *) &serverAddress,
              sizeof(serverAddress)) < 0) 
			  perror("Server error: On binding - bind()\n");
			  
	// Listening on client connection
	listen(sockfd, 5);
	 
	// Connection with client
	clientAddressLength = sizeof(clientAddress);
	newsockfd = accept(sockfd, (struct sockaddr *) &clientAddress, &clientAddressLength);
	if (newsockfd < 0) 
		perror("Server error: On accept");

	// Waiting on message	
	bzero(buffer,256);
	n = read(newsockfd,buffer,255);
	if (n < 0) perror("Server error: Reading from socket - read()\n");
	printf("Here is the message: %s\n",buffer);
	n = write(newsockfd,"I got your message",18);
	if (n < 0) perror("Server error: Writing to socket - write()\n");

    return 1;
}   
