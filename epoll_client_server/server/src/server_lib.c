#include "server_lib.h"


// Function creating socket ( returns file descriptor)
int CreateSocket(){
  int msock = socket(AF_INET,SOCK_STREAM,0);
  if(msock==-1){
    printf("[X] Failed to create socket : %s \n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("[*] Succesfully created socket!\n");
  return msock;
}
// Function binding
struct addrinfo* BindSocket(int msocket,char* ipaddr,char* portnum){
  // Obtain address information
  struct addrinfo* addrinfo;
  if(getaddrinfo(ipaddr,portnum,NULL,&addrinfo)!=0){
    printf("[X] Failed to obtain address information : %s \n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("[*] Succesfully obtained address information!\n");
  // Socket binding
  if(bind(msocket,addrinfo->ai_addr,addrinfo->ai_addrlen)!=0){
      printf("[X] Failed to bind socket : %s \n",strerror(errno));
      exit(EXIT_FAILURE);
  }
  printf("[*] Succesfully bound socket!\n");
  return addrinfo;
}

void ListenSocket(int msocket){
  // Listen on Socket
  if(listen(msocket,BACKLOG)!=0){
    printf("Failed to listen socket : %s \n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("[*] Listen on socket succesfull!\n");
}

// Add new client to the list
void AddClient(struct client* clist,int csock,char* cname){
  if(strlen(cname)>0){ // name validation
    int i=0;
    while(clist[i].status != 0){
      if(strcmp(clist[i].name,cname) == 0){
        printf("[X] Client with name %s is already on the list",cname);
        return;
      }
      i++;
    }
    if(i>=BACKLOG){
      printf("[X] Cannot add another client reached limit for connections");
      return;
    }
    memset(clist[i].name,0,255);
    strcat(clist[i].name,cname);
    clist[i].status = 1;
    clist[i].csock = csock;
  }
  else{
    printf("[X] Couldnt connect client (invalid name)");
  }

}

// Remove client from list
void RemoveClient(struct client* clist,char* cname){
  int i=0;
  while(strcmp(clist[i].name,cname) != 0){
    i++;
  }
  memset(clist[i].name,0,255);
  clist[i].status = 0;
  clist[i].csock = -1;

}

char* GetNameBySock(struct client* clist,int fd){
  for(int i=0;i<BACKLOG;i++){
    if(clist[i].csock == fd){
      return clist[i].name;
    }
  }
  return "";
}
