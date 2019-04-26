#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <signal.h>
#include <arpa/inet.h>
#include <time.h>
#define PORT 8100 

int getRandomInteger(int n){
  
  srand(time(0));
  return(rand()%n + 1);
}

int main(int argc, char const *argv[]) 
{ 
    int server_fd, new_socket, portNumber;
    struct sockaddr_in address; 
    int opt = 1; 
    int addrlen = sizeof(address);
	int status;
	char message[100] = "Got a message from Server";

	if(argc != 2){
	printf("Call model: %s <Port Number>\n", argv[0]);
	exit(0);
	}

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	sscanf(argv[1], "%d", &portNumber);
	address.sin_port = htons((uint16_t)portNumber);

	bind(server_fd, (struct sockaddr *) &address, addrlen);
	listen(server_fd, 5);
	
	while(1){
		printf("Waiting for a client to join with\n");

		if ((new_socket = accept(server_fd, NULL,  
						   NULL))<0) 
		{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
		} 

		printf("-----------Got a client, start playing---------\n");

		//Writing a message to client
		write(new_socket, &message, 100);

		if(!fork()){
			child(new_socket);
		}
		//wait(&status);
		close(new_socket);
	}
}

void child(int new_socket){	
	
	int dice = 0;
	int cpoints = 0;
	int spoints = 0;
	
	while(1){
		
		if(spoints < 100){
		
		dice = getRandomInteger(10);
		spoints += dice;		
		}
		else{
			printf("\n\n----------Server won the match with %d points--------\n\n", spoints);
			exit(0);
		}		
		
		write(new_socket, &dice, sizeof(dice));
		
		read(new_socket, &cpoints,sizeof(cpoints));	
		
		if(cpoints == 0){
			printf("\n\n---------Client has won the match-------\n\n");
			exit(0);
		}

	}
}
	
	
