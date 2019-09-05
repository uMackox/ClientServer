#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <syslog.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
// Client structure
struct client_struct{
  char name[255]; // Client name
  int con_sock; // Connection file descriptor
  int status; // Status of client
};


#define USAGE_STR "./server -q -p [portnumber]\n"
#define SERVER_NAME "KomSrv"
#define BSIZE 1024
#define LIMIT 10

// Add client to list
void AddClient(struct client_struct** clist,char* name,int fd);

// Retrive current client list
char* GetClientList(struct client_struct* clist);

// Get client name based on socket
char* GetNameBySock(struct client_struct* clist, int fd);

// Remove client from list
void RemoveClient(struct client_struct** clist, char* name);

// Send message to client
void SendMsg(char* msg,struct client_struct* clist);

// Kill other (running) instances of server
int HandleOtherInstance(char flags);
