/*
 * common headers
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <sys/time.h>


#define BUFSIZE 30

/* check the number of args */
void checkArgc(int& argc, int num);

/* mySocket - socket() */
int mySocket(int& listenfd, 
			 int family=AF_INET,
			 int type=SOCK_STREAM,
			 int protocol=0);

/* init sockaddr */
void initSockAddr(struct sockaddr_in &servaddr,
				  const char* ip,
				  int port);


/* myBind - bind() */
void myBind(int& sockfd,
			const struct sockaddr* myaddr,
			socklen_t addrlen);

/* myListen - listen() */
void myListen(int sockfd,
			  int backlog);


/* myAccept - accept() */
int myAccept(int& sockfd,
			 struct sockaddr* cliaddr,
			 socklen_t *clilen);


/* myConnect - connect() */
void myConnect(int& sockfd,
			   struct sockaddr* servaddr,
			   socklen_t addrlen);
