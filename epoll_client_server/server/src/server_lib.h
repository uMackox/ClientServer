#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#define BACKLOG 10

int CreateSocket();
void BindSocket(int,char*, char*);
void ListenSocket(int);
