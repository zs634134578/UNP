#include "common.h"


void checkArgc(int& argc, int num) {
	if ( argc <= num ) {
		fprintf(stderr,
				"ip and port info are needed: %d\n",
				argc);
		exit(0);
	}
}


int mySocket(int& listenfd, 
			 int family,
			 int type,
			 int protocol) {
	listenfd = socket(family, type, protocol);
	if ( listenfd < 0 ) {
		fprintf(stderr,
				"Initialize socket failed: %s\n",
				strerror(errno));
		exit(0);
	}
	
}


void initSockAddr(struct sockaddr_in& servaddr,
				  const char* ip,
				  int port) {
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton( AF_INET, 
			   ip,
			   &servaddr.sin_addr);
	servaddr.sin_port = htons( port );
}


void myBind(int& sockfd,
			const struct sockaddr* myaddr,
			socklen_t addrlen) {
	int ret = bind(sockfd, myaddr, addrlen);
	if ( ret < 0 ) {
		fprintf(stderr,
				"bingding failed: %s\n",
				strerror(errno));
		exit(0);
	}
}

void myListen(int sockfd,
			  int backlog) {
	int ret;
	ret = listen(sockfd, backlog);
	if ( ret < 0 ) {
		fprintf(stderr,
				"listen socket(%d) failed: %s\n",
				sockfd,
				strerror(errno));
		exit(0);	
	}
}


int myAccept(int& sockfd,
			  struct sockaddr* cliaddr,
			  socklen_t *clilen) {
	int ret;
	ret = accept(sockfd, cliaddr, clilen);
	if (ret < 0) {
		
		if (errno == EINTR) // restart when meet with EINTR signal
			return myAccept(sockfd, cliaddr, clilen);

		fprintf(stderr,
				"accept faled: %s\n",
				strerror(errno));
		exit(0);\
	}
	else
		return ret;
}


void myConnect(int& sockfd,
			  struct sockaddr* servaddr,
			  socklen_t addrlen) {
	int ret;
	ret = connect(sockfd, servaddr, addrlen);
	if( ret < 0 ) {
		fprintf(stderr,
				"connect failed: %s\n",
				strerror(errno));
		exit(0);
	}
	else 
		return;
}
