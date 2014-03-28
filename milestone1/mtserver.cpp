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

    /* 1 declare socket*/
    int listenfd, connfd;
    int ret;
    
    /* 2 initialize listen socket*/
    mySocket(listenfd);
    
    /* 3 server address */
    struct sockaddr_in servaddr;
    initSockAddr(servaddr, ip, port);
    
    /* 4 bind */
    myBind(listenfd,
	 	   (struct sockaddr*)&servaddr,
           sizeof(servaddr));
    
    /* 5 listen */
    myListen(listenfd, 5);
    
    /* handle SIGCHLD signal*/
    signal(SIGCHLD, handle_sigchild);
    
    /* 6 waiting for connecting */
    pid_t chipid;
    socklen_t clilen;
    struct sockaddr_in cliaddr;
    
    for(;;) {
	 	clilen = sizeof(cliaddr);
        std::cout << "Waiting for connecting ..." << std::endl;
        connfd = myAccept(listenfd, 
		 				  (struct sockaddr*)&cliaddr, 
                          &clilen); 
        printf("Connection %d established...\n", listenfd);
        if ( (chipid=fork()) == 0 ) {
		 	handle_recv(connfd);
        }
    }

} 


void handle_recv(int connfd) {
     
    char recvbuf[BUFSIZE];
    
    while(1) {
        memset( recvbuf, '\0', BUFSIZE );
        if ( recv(connfd, recvbuf,BUFSIZE,0) != 0) {
		 	if (!strcmp(recvbuf, "exit"))
                break;
            fprintf(stderr,"recv msg: %s\n", recvbuf);
            send(connfd, recvbuf, strlen(recvbuf), 0);
            fprintf(stderr,"send back: %s\n\n", recvbuf);
        }
    }
    close(connfd);
    exit(0);
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
