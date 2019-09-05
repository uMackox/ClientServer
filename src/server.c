#include "server_lib.h"


int main(int argc, char* argv[]){
  /*
  Basic communicator program based on epoll
  Options:
    -q - checks if there is already running instance of server. On find it kills running instance
    -p [portnumber] - sets port that server will be listening on
  */
  char opt;
  char flags=0;
  char* portnum= "10";
  // Option checking
  while((opt = getopt(argc,argv, "qp:"))!=-1){
    switch(opt){
      case 'q' : flags += 1;
                break;
      case 'p' : flags += 2;
                 portnum = optarg;
       break;
    }
  }

  // No arguments given
  if(flags == 0){
    printf(USAGE_STR);
    return 0;
  }

  // Handling -q option
  if (flags&0x01){
    if(HandleOtherInstance(flags)){
      return 0;
    }
  }

  // Server daemonization
  pid_t dmn = fork();
  if(dmn>0){
    exit(EXIT_SUCCESS);
  }
  umask(0);
  openlog(SERVER_NAME,LOG_CONS|LOG_PID,LOG_DAEMON); // Openning logs
  /*
  Logs checking command : tail -10 /var/log/syslog
  */
  setsid();
  syslog(0,"Obtained PID : %d ",getpid());
  if(chdir("/")){ // changing working directory
    exit(0);
  }
  else{
    syslog(0,"Changed working directory !");
  }
  // Closing std file descriptors
  close(0);
  close(1);
  close(2);

  // Client list initialization
  struct client_struct* clist;
  clist = (struct client_struct*) calloc (1,sizeof(struct client_struct));
  clist[0].status = -1;

  // Creating main listening socket
  int msocket = socket(AF_INET,SOCK_STREAM,0);
  if(msocket==-1){
    printf("Error creating socket : %s \n",strerror(errno));
    exit(-1);
  }
  syslog(0,"Socket created!\n");

  // Preparing structure containing adress
  struct addrinfo* sinf;
  getaddrinfo("127.0.0.1",portnum,NULL , &sinf);
  if(bind(msocket,sinf->ai_addr,sinf->ai_addrlen)){
    syslog(0,"Error binding socket : %s \n",strerror(errno));
    exit(-1);
  }
  syslog(0,"Socket bound! \n");


  // Prepare connection queue
  if(listen(msocket, LIMIT)){
    syslog(0,"Error listening socket : %s \n",strerror(errno));
    exit(-1);
  }
  syslog(0,"Listen succrsfull! \n");
  struct epoll_event ev, events[LIMIT];
  int epollfd;
  epollfd = epoll_create1(0);
  if(epollfd==-1){
    syslog(0,"Error creating epoll socket : %s \n",strerror(errno));
    exit(-1);
  }

  ev.events = EPOLLIN;
  ev.data.fd = msocket;
  if(epoll_ctl(epollfd,EPOLL_CTL_ADD, msocket,&ev)==-1){
    syslog(0,"Error connecting epoll to socket : %s \n",strerror(errno));
    exit(-1);
  }
  int csocket; // connection socket
  int efd; // epoll event number of ready file descriptors
  for(;;){
    efd = epoll_wait(epollfd,events,LIMIT,-1);
    if(efd==-1){
      syslog(0,"Error binding socket : %s \n",strerror(errno));
      exit(-1);
    }
    for(int i=0;i<efd;i++){
      if(events[i].data.fd == msocket){
        csocket = accept(msocket,(struct sockaddr *)sinf->ai_addr,&sinf->ai_addrlen);
        if(csocket ==-1){
          syslog(0,"Error connecting client : %s \n",strerror(errno));
          exit(-1);
        }
        ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = csocket;
        if(epoll_ctl(epollfd,EPOLL_CTL_ADD,csocket,&ev)==-1){
          syslog(0,"Error connecting epoll to client : %s \n",strerror(errno));
          exit(-1);
        }

      }
      else{
        char* list;
        char buff[BSIZE];
        memset(buff,0,BSIZE);
        int readin = recv(events[i].data.fd,buff,sizeof(buff)-1,0);
        syslog(0,"Recived %d bytes from client %d",readin,events[i].data.fd);
        buff[readin-2]='\0';
        char preamb = buff[0];
        syslog(0,"Recived message from client : %s preambule : %c!",buff,preamb);

        switch(preamb){
          case 'I': // Introduction
            AddClient(&clist,buff+2,events[i].data.fd);
            break;
          case 'L': // Client list request
            //strcat(list, GetClientList(clist));
            list = GetClientList(clist);
            send(events[i].data.fd,list,strlen(list),0);
            syslog(0,"Sent list : %s ",list);
            free(list);
            break;
          case 'M': // Client messageges
            SendMsg(buff+2,clist);
            break;
          default:
            RemoveClient(&clist,GetNameBySock(clist,events[i].data.fd));
            syslog(0,"Received unknown preambule");
        }
      }

    }
  }
	free(clist);
  closelog();
  return 0;
}
