#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <sys/epoll.h>

#define BACKLOG 10
#define BSIZE 1024

struct client{
  int csock;
  char name[255];
  char status;
};

int CreateSocket();
struct addrinfo* BindSocket(int,char*, char*);
void ListenSocket(int);
void AddClient(struct client*,int,char*);
