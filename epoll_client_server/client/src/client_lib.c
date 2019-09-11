#include "client_lib.h"

// Create socket
int CreateSocket(){
  int msock = socket(AF_INET,SOCK_STREAM,0);
  if(msock==-1){
    printf("[X] Error creating socket : %s \n",strerror(errno));
    exit(-1);
  }
  printf("[*] Succesfully created socket !\n");
  return msock;
}
// Connect socket to server
void ConnectSocket(int msocket,const char* ipaddress,const char* portnum){
  struct addrinfo* serverinfo;
  getaddrinfo(ipaddress,portnum,NULL,&serverinfo);
  if(connect(msocket, serverinfo->ai_addr,serverinfo->ai_addrlen)){
    printf("[X] Error connecting: %s \n",strerror(errno));
    exit(-1);
  }
  printf("[*] Connection succesfull !\n");
}

// Message receiving function
void* ReceiveMsgs(void* args){
  int* msock = (int*)  args;
  char buff[BSIZE];
  for(;;){
    memset(buff,0,BSIZE);
    recv(*msock,buff,sizeof(buff)-1,0);
    printf("\nReceived message :\n %s \n",buff);
  }
}
