#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define SERVER_PORT 12346
#define BUF_SIZE 4096
#define UNIX_EPOCH 2208988800

int main(int argc, char const *argv[])
{
	int udpSocket,nBytes;
	time_t buffer;
	uint32_t ct;
	struct sockaddr_in serverAddr,clientAddr;
	socklen_t client_addr_size;

	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero); 

	udpSocket = socket(AF_INET,SOCK_DGRAM,0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(udpSocket,(struct sockaddr *) &serverAddr,sizeof(struct sockaddr_in)) == -1 )
	{
		printf("failed to bind\n");
		exit(0);
	}	

	
	while(1){
		client_addr_size = sizeof(struct sockaddr_in);
		nBytes = recvfrom(udpSocket,&buffer,1024,0,(struct sockaddr *)&clientAddr, &client_addr_size); /// lägg till clientAddr.
		if (nBytes < 0)
		{
			printf("could not recive datagram\n");
			exit(0);
		}
		ct = time(NULL) + UNIX_EPOCH;
		printf("%d\n",ct);
		if(sendto(udpSocket,&ct,sizeof(ct),0,(struct sockaddr *)&clientAddr,client_addr_size) == -1)
		{
			printf("failed to transmitt data.\n");
			exit(0);
		}
	}
	close(udpSocket);
	return 0;
}