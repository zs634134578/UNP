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
    
	signal(SIGALRM, sig_alrm);

    while(1) {
	 	memset( sendbuf, '\0', BUFSIZE );
        memset( recvbuf, '\0', BUFSIZE );
        
        printf("%s", "send msg:");
        gets(sendbuf);

        if (strlen(sendbuf) > 0)
		 	send(sockfd,sendbuf,strlen(sendbuf),0);

        if ( !strcmp(sendbuf, "exit"))
            break;

		alarm(5);
        if (recv(sockfd,recvbuf,BUFSIZE,0) > 0) {
			alarm(0);
            printf("recv back:%s\n\n", recvbuf);
		}
		else {
			if (errno == EINTR)
				fprintf(stderr,
						"socket timeout\n");
			else
			    fprintf(stderr,
						"receive error\n");
		}
	}
    close( sockfd );
    return;
}


static void sig_alrm(int signo) {
	fprintf(stderr,
			"recv SIGALRM, return.\n");
	return;
}
