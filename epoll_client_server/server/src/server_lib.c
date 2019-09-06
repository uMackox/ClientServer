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
void BindSocket(int msocket,char* ipaddr,char* portnum){
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
}

void ListenSocket(int msocket){
  // Listen on Socket
  if(listen(msocket,BACKLOG)!=0){
    printf("Failed to listen socket : %s \n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("[*] Listen on socket succesfull!\n");
}
