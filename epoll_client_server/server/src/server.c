#include "server_lib.h"

int main(int argc,char* argv[]){
  // Creating socket
  int msock = CreateSocket();
  // Binding ip adress and port number to socket
  BindSocket(msock, argv[1],argv[2]);
  // Listening on socket
  ListenSocket(msock);

  close(msock);
  return 0;
}
