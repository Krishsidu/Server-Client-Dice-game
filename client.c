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

    char buffer[1024] = {0};
	int cpoints=0;
	int spoints=0;
    int sdice, cdice;
	char option;
	char message[100] = "Message from Client";
	
	int server, portNumber;
	socklen_t len;
	struct sockaddr_in servAdd;     // server socket address

	if(argc != 3){
	printf("Call model: %s <IP Address> <Port Number>\n", argv[0]);
	exit(0);
	}

	if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	 fprintf(stderr, "Cannot create socket\n");
	 exit(1);
	}

	servAdd.sin_family = AF_INET;
	sscanf(argv[2], "%d", &portNumber);
	servAdd.sin_port = htons((uint16_t)portNumber);

	if(inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0){
	fprintf(stderr, " inet_pton() has failed\n");
	exit(2);
	}

	if(connect(server, (struct sockaddr *) &servAdd, sizeof(servAdd))<0){
	fprintf(stderr, "connect() has failed, exiting\n");
	exit(3);
	}
	
	if(-1==read(server, &message, 100))
		perror("Read Failed");
	printf("%s\n",message);
	
	printf("Press Enter to Start the Game or press q to quit\n");
	
	while(1){
		
		scanf("%c",&option);
		
		if( option == 'q' )
			exit(0);
		
		if(option != 'q'){
		
			read(server, &sdice, sizeof(sdice));
			spoints += sdice;
			
			printf("Server's dice value: %d\n",sdice);
			printf("Server's total: %d\n", spoints);
			
			if(spoints >= 100){		
				printf("\n\n------------Server has Won the match with %d points----------\n\n",spoints);
				exit(0);
			}
			else{
				cdice = getRandomInteger(10);
				cpoints += cdice;
				printf("Client's dice value: %d\n",cdice);
			}
			
			printf("Client's total: %d\n", cpoints);
			
			if(cpoints >= 100){
				printf("\n\n------------Client has won the match with %d points-----------\n\n",cpoints);
				cpoints = 0;
				write(server, &cpoints, sizeof(cpoints));
				exit(0);
			}
			write(server, &cpoints, sizeof(cpoints));
		}
	}
	
}