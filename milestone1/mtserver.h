#ifndef MTSERVERH
#define MTSERVERH

#include "common.h"

// handle receive msg, and response
void handle_recv(int connfd);

// handle zombie function
void handle_sigchild(int signo);

#endif
