#include "server_lib.h"

int main(int argc,char* argv[]){
  // Creating socket
  int msock = CreateSocket();
  // Binding ip adress and port number to socket
  struct addrinfo* addr;
  addr = BindSocket(msock, argv[1],argv[2]);
  // Listening on socket
  ListenSocket(msock);

  // Epoll handling
  struct epoll_event ev, events[BACKLOG];
  int epollfd,nfd,csock;
  // Creating epoll instance
  epollfd = epoll_create1(0);
  if(epollfd==-1){
    printf("[X] Error creating epoll instance : %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }

  // Connect epoll to listening Socket
  ev.events = EPOLLIN;
  ev.data.fd = msock;
  if(epoll_ctl(epollfd,EPOLL_CTL_ADD,msock,&ev)==-1){
    printf("[X] Error connecting epoll to socket (epoll_ctl) : %s\n",strerror(errno));
    exit(EXIT_FAILURE);
  }
  // Main epoll loop

  struct client* clist; // client list preparation
  clist = (struct client*) calloc(BACKLOG,sizeof(struct client));

  char buff[BSIZE];
  for(;;){
    nfd = epoll_wait(epollfd,events,BACKLOG,-1);
    if(nfd == -1){
      printf("[X] Error on epoll wait : %s\n",strerror(errno));
      exit(EXIT_FAILURE);
    }
    for(int i=0;i<nfd;i++){
      if(events[i].data.fd==msock){
        csock = accept(msock, (struct sockaddr *) addr->ai_addr,&addr->ai_addrlen);
        if(csock == -1){
          printf("[X] Error accepting connection : %s\n",strerror(errno));
          exit(EXIT_FAILURE);
        }
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = csock;
        if(epoll_ctl(epollfd,EPOLL_CTL_ADD,csock,&ev)==-1){
          printf("[X] Error connecting epoll to client : %s \n",strerror(errno));
          exit(-1);
        }
      }
      else{
        memset(buff,0,BSIZE);
        recv(events[i].data.fd,buff,sizeof(buff)-1,0);
        if(strncmp(buff,"",1)==0){
          printf("Client %s closed connection\n",GetNameBySock(clist,events[i].data.fd));
          RemoveClient(clist,GetNameBySock(clist,events[i].data.fd));
          close(events[i].data.fd);
        };
        else{
          char preamb;
          preamb = buff[0];
          switch(preamb){
            case 'I': // Introduction
              AddClient(clist,events[i].data.fd,buff+2);
              printf("Client %s just connected\n",buff+2);
              break;
            default:
              printf("\nReceived from %s :\n %s\n",GetNameBySock(clist,events[i].data.fd),buff);
          }

        }
      }
    }
  }
  close(msock);
  return 0;
}
