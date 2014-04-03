/*
  client
*/

#include "mtclient.h"

int main(int argc, char* argv[])
{   
	if (argc <=2 ) {
        std::cout << "server ip and port needed." << std::endl;
        return 1;
    }
    
    int port = atoi(argv[2]);
    char* ip = argv[1];
	
    int sockfd;
    struct sockaddr_in servaddr;
	
    mySocket(sockfd);

    initSockAddr(servaddr,ip, port);
    
    myConnect(sockfd,
              (struct sockaddr*)&servaddr,
              sizeof(servaddr));
    
    handle_msg(sockfd);
    exit(0);
    
}


void handle_msg(int sockfd) {

    char sendbuf[BUFSIZE];
    char recvbuf[BUFSIZE];
    

    while(1) {
	 	memset( sendbuf, '\0', BUFSIZE );
        memset( recvbuf, '\0', BUFSIZE );
        
        printf("%s", "send msg:");
        gets(sendbuf);

        if (strlen(sendbuf) > 0)
		 	send(sockfd,sendbuf,strlen(sendbuf),0);

        if ( !strcmp(sendbuf, "exit"))
            break;
		
		if (readable_timeo(sockfd, 5) == 0) {
			fprintf(stderr,
					"socket timeout\n");
		}
		else{
			recv(sockfd,recvbuf,BUFSIZE,0);
			printf("recv back:%s\n\n", recvbuf);
		}
	}
    close( sockfd );
    return;
}

int readable_timeo(int fd, int sec) {
	fd_set rset;
	struct timeval tv;
	
	FD_ZERO(&rset);
	FD_SET(fd, &rset);

	tv.tv_sec = sec;
	tv.tv_usec = 0;

	return select(fd+1, &rset, NULL, NULL, &tv);
}



