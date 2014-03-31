/*
 * 功能描述：
 *   客户端与服务器端进行TCP连接
 *   客户端从标准输入（键盘）读入一行文本，发送给服务器
 *   服务器从网络输入读取该行文本，并回射给客户
 *   客户从网络输入读入这行文本，并显示在标准输出（终端显示器）上
 *   支持并发（多进程实现）
 */

#include "mtserver.h"

int main(int argc, char* argv[])
{
    checkArgc(argc, 2);
    
    const char* ip = argv[1];
    int port = atoi( argv[2] );

    /* declare socket*/
    int listenfd, connfd, sockfd;
    int ret;
    
    /* initialize listen socket*/
    mySocket(listenfd);
    
    /* server address */
    struct sockaddr_in servaddr;
    initSockAddr(servaddr, ip, port);
    
    /* bind */
    myBind(listenfd,
	 	   (struct sockaddr*)&servaddr,
           sizeof(servaddr));
    
    /* listen */
    myListen(listenfd, 5);
    
    /* handle SIGCHLD signal*/
    //signal(SIGCHLD, handle_sigchild);
    
    /* waiting for connecting */
    pid_t chipid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;

	/* select initialize */
	int maxfd, maxi, i;
	bool toclose;
	int nready, client[FD_SETSIZE];
	fd_set rset, allset;
	
	maxfd = listenfd;
	maxi = -1;
	for ( i=0; i < FD_SETSIZE; i++ )
		client[i] = -1;

	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);
    
	printf("Waiting for connecting...\n");
	
    for(;;) {
		rset = allset;
		if ( (nready=select(maxfd+1, &rset, NULL, NULL, NULL)) < 0 ) {
			fprintf(stderr,
					"select failed.%s\n",
					strerror(errno));
			continue;
		}
		
		/* handle listen fd and no recv or no respond */
		if (FD_ISSET(listenfd, &rset)) {
			clilen = sizeof(cliaddr);
			connfd = myAccept(listenfd,
							  (struct sockaddr*)&cliaddr,
							  &clilen);
			printf("Connection is established with sockfd: %d\n",
				   connfd);
			for ( i = 0; i < FD_SETSIZE; i++) {
				if ( client[i] < 0 ) {
					client[i] = connfd;
					break;
				}
			}
			
			if (i == FD_SETSIZE) {
				fprintf(stderr,
						"too many clients\n"
						);
				break;
			}
			
			FD_SET( connfd, &allset );
			if ( connfd > maxfd ) {
				maxfd = connfd;
			}
			if ( i > maxi) {
				maxi = i;
			}
			
			if (--nready <= 0) {
				continue;
			}
		}
		
		/* handle accept fds(client[]) and handle recv or respond msg */
		for ( i = 0; i <= maxi; i++) {
			if ( (sockfd = client[i]) < 0 )
				continue;
			if ( FD_ISSET(sockfd, &rset) ) {
				if( (toclose = handle_recv(sockfd))) {
					printf("Client close this connection: %d\n" ,
						   sockfd);
					close(sockfd);
					FD_CLR(sockfd, &allset);
					client[i] = -1;
				}
				
				if (--nready <= 0) 
					break;
			}
		}
    }
}


bool handle_recv(int connfd) {
     
    char recvbuf[BUFSIZE];

	memset( recvbuf, '\0', BUFSIZE );
	if ( recv(connfd, recvbuf,BUFSIZE,0) != 0) {
		if (!strcmp(recvbuf, "exit"))
			return true;
		fprintf(stderr,"recv msg: \"%s\" from connfd:%d\n", recvbuf, connfd);
		send(connfd, recvbuf, strlen(recvbuf), 0);
		fprintf(stderr,"send back: \"%s\" to connfd:%d\n\n", recvbuf, connfd);
	}
	else
		return true;
	return false;
}


void handle_sigchild(int signo)
{
    pid_t pid;
    int stat;
    
    while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0 ) {
        fprintf(stderr,
                "child %d terminated\n",
                pid);
        return;
    } 
}
