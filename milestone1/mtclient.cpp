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
    
    int maxfdpl, ret;
    fd_set rset;
	int normalTermi = 0;
    
    FD_ZERO(&rset);

    while(1) {
	 	memset( sendbuf, '\0', BUFSIZE );
        memset( recvbuf, '\0', BUFSIZE );
        
		if (normalTermi == 0)
			FD_SET( 0, &rset );

        FD_SET( sockfd, &rset );		
		maxfdpl = sockfd + 1;
		
		if(DEBUG)
			printf("Debug: waiting in select\n");
		if ( select( maxfdpl, &rset, NULL, NULL, NULL) < 0 ) {
			fprintf(stderr,
					"select failed.%s\n",
					strerror(errno));
		}
		if(DEBUG)
			printf("Debug: after select\n");

		if (FD_ISSET( sockfd, &rset )) {
			if (recv(sockfd, recvbuf, BUFSIZE, 0) == 0) {
				
				if(DEBUG)
					printf("Debug: ready to quit, normalTermi: %d\n" ,
						   normalTermi);
				
				if (normalTermi == 1) {
					printf("handle_msg: normal terminated.\n");
					return;
				}
				else {
					printf("handle_msg: server terminated.\n");
					exit(0);
				}
			}
			fprintf(stderr,
					"recv back: %s\n",
					recvbuf);
		}
		else if ( FD_ISSET( 0, &rset ) ) {
			gets(sendbuf);
			if (strlen(sendbuf) > 0) {
				send(sockfd, sendbuf, strlen(sendbuf), 0);
				if ( !strcmp(sendbuf, "exit") ) {
					normalTermi = 1;
					shutdown(sockfd, SHUT_WR);
					FD_CLR(0, &rset);
					continue;
				}
			}
		}
    }
    close( sockfd );
    return;
}
