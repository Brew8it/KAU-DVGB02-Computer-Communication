#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define SERVER_PORT 12346
#define BUF_SIZE 4096
#define UNIX_EPOCH 2208988800

int main(int argc, char const *argv[])
{
	int clSocket, nBytes = 0;
	time_t servertime;
	uint32_t ct;
	struct sockaddr_in serverAddr;
	socklen_t addr_size;

	char Empty[10];

	 memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	clSocket = socket(AF_INET,SOCK_DGRAM,0);
	if (clSocket == -1)
	{
		printf("Cant create socket\n");
		exit(0);
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);


	addr_size = sizeof(struct sockaddr_in);

	while(1){
		nBytes = strlen(Empty);
		sendto(clSocket,&Empty,nBytes,0,(struct sockaddr *)&serverAddr,addr_size);
		nBytes = recvfrom(clSocket,&ct,BUF_SIZE,0,(struct sockaddr *)&serverAddr,&addr_size);
		printf("%d\n",ct);

		servertime = ct - UNIX_EPOCH;
		printf("%ld\n",servertime);
		printf("Current time = %s", ctime(&servertime));
		close(clSocket);
		break;
	}
	return 0;
}