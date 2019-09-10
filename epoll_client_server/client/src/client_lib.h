#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define BSIZE 1024

int CreateSocket();
void ConnectSocket(int,const char*,const char*);
void* ReceiveMsgs(void* args);
