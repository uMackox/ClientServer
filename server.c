#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>

#define BACKLOG 10
#define BSIZE 255
int main(int argc,char* argv[]){
    // Create socket for communication
    int msock = socket(AF_INET,SOCK_STREAM,0);
    if(msock==-1){
      printf("Failed to create socket : %s \n",strerror(errno));
    }
    // Obtain address information
    struct addrinfo* addrinfo;
    if(getaddrinfo("127.0.0.1",argv[1],NULL,&addrinfo)!=0){
      printf("Failed to obtain address information : %s \n",strerror(errno));
    }

    // Socket binding
    if(bind(msock,addrinfo->ai_addr,addrinfo->ai_addrlen)!=0){
        printf("Failed to bind socket : %s \n",strerror(errno));
    }

    // Listen on Socket
    if(listen(msock,BACKLOG)!=0){
      printf("Failed to listen socket : %s \n",strerror(errno));
    }

    // Prepare structure for client connection
    struct sockaddr clientaddrinf;
    socklen_t csize;
    int cfd = accept(msock,&clientaddrinf,&csize);
    if(cfd < 0 ){
      printf("Accept failed \n");
      return -1;
    }

  char buff[BSIZE];
  for(;;){
    memset(buff,0,BSIZE);
    recv(cfd,buff,sizeof(buff)-1,0);
    printf("\nReceived message :\n %s \n",buff);
  }

  return 0;
}
