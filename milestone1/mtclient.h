#ifndef MTCLIENTH
#define MTCLIENTH

#include "common.h"

void handle_msg(int sockfd);

static void sig_alrm(int signo);

int readable_timeo(int fd, int sec);

#endif
