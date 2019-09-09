#include "client_lib.h"

int main(int argc, char const *argv[]) {

  // Creating socket
  int msock = CreateSocket();

  ConnectSocket(msock,argv[1],argv[2]);
  // Introduction
  char msg[255]="I ";
  int msglen;
  strcat(msg,argv[3]);
  msglen = strlen(msg);
  msg[msglen+1]='\r';
  msg[msglen+2]='\n';
  send(msock,msg,msglen+2,0);



  // Message buffor preparation
  size_t buffsize;
  char* buff;
  // Main client loop
  for(;;){
    printf("Type message (exit to end):\n");
    getline(&buff,&buffsize,stdin);
    if(strncmp(buff,"exit",4)==0){
      break;
    }
    send(msock,buff,strlen(buff),0);
  }

  close(msock);
  return 0;
}
