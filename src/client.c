#include "client_lib.h"


int main(int argc, char* argv[]){

  int msock = socket(AF_INET,SOCK_STREAM,0);
  if(msock==-1){
    printf("Error creating socket : %s \n",strerror(errno));
    exit(-1);
  }

  printf("Socket created! \n");
  // Prepare adress structure
  struct addrinfo* serverinfo;
  getaddrinfo(argv[1],argv[2],NULL,&serverinfo);
  if(connect(msock, serverinfo->ai_addr,serverinfo->ai_addrlen)){
    printf("Error connecting: %s \n",strerror(errno));
    exit(-1);
  }
  size_t buffsize;
  char* buff;
  // Main client loop
  for(;;){
    printf("\n");
    getline(&buff,&buffsize,stdin);
    send(msock,buff,strlen(buff),0);
  }



  return 0;
}
