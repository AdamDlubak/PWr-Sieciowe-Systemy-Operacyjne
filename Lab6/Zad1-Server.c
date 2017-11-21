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
#include <dirent.h>

int main(int argc, char * argv[]){ 

	int sockfd, newsockfd, portNumber, clientAddressLength;
	char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;
	int n;


    int res;
    struct dirent * ent;
    DIR * dir = NULL;


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
	if (newsockfd < 0) { perror("Server error: On accept"); exit(0); }

	while(1) {
		// bzero(buffer,256);
		// n = read(newsockfd,buffer,255);
		// if (n < 0) perror("Server error: Reading from socket - read()\n");
		// printf("Dostalem komende:\n %s", buffer);
		// switch(atoi(buffer)) {

		// 	case 1:
				bzero(buffer,256);
				n = read(newsockfd,buffer,255);
				if (n < 0) perror("Server error: Reading from socket - read()\n");
				printf("Here is the message from client: %s\n",buffer);
				// n = write(newsockfd,"Server answer: I got your message",18);
				// if (n < 0) perror("Server error: Writing to socket - write()\n");
		// 		break;
		// 	case 2:
		// 		// Opening Direcotry
		// 	    dir = opendir("./exampleDir");
        //         if(dir == NULL) perror("Server error: Open directory - opendir()");
		// 		printf("\tDirectory was opened\n");
				
		// 		while ((ent = readdir (dir)) != NULL) {
    	// 			printf ("%s\n", ent->d_name);
		// 		}

		// 		printf("\tSending content of directory...\n");

		// 		// Closing directory
  		// 		res = closedir(dir);
		// 		if(res < 0) perror("Server error: Close directory - closedir()");
		// 		printf("\tDirectory was closed\n");
				
		// 		break;
		// }


	}
    return 1;
}   
