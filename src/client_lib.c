#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define USAGE_STR "./client -a [adres_ip] -p [port] -n [nazwa]\n"
#define BSIZE 1024
void* ReceiveMsgs(void* arg){
  int* msocket = (int*) arg;
  //printf("Thread here my socket is : %d\n",*msocket);
  char buff[BSIZE];
  for(;;){
    memset(buff,0,BSIZE);
    recv(*msocket,buff,sizeof(buff)-1,0);
    printf("\nReceived message :\n %s \n",buff);

  }
  return NULL;
}

int main(int argc, char* argv[]){
  //printf("Server here\n");
  char opt;
  char flags=0;
  char*  ip;
  char* name;
  char* portnum;
  // Option checking
  while((opt = getopt(argc,argv, "a:p:n:"))!=-1){
    switch(opt){
      case 'a' : flags += 1;
                ip = optarg;
                break;
      case 'p' : flags += 2;
                portnum = optarg;
                break;
      case 'n' : flags += 4;
                name = optarg;
                break;
    }
  }
  if(flags < 7){
    printf(USAGE_STR);
    return 0;
  }

  int msocket = socket(AF_INET,SOCK_STREAM,0);
  if(msocket==-1){
    printf("Error creating socket : %s \n",strerror(errno));
    exit(-1);
  }
  printf("Socket created!\n");
  // Preparing address structure
  struct addrinfo* sinf;
  getaddrinfo(ip,portnum,NULL , &sinf);
  if(connect(msocket,sinf->ai_addr,sinf->ai_addrlen)){ // connecting to server
    printf("Error binding socket : %s \n",strerror(errno));
    exit(-1);
  }
  // Creating message receiving thread
  pthread_t thread;
  if(pthread_create(&thread,NULL,ReceiveMsgs,(void*)&msocket)){
      printf("Failed to create Receiver pthread");
  }

  size_t bsize;
  char* buff;

  // Create introduction
  char msg[255] ="I ";
  int msglen;
  strcat(msg,name);
  msglen = strlen(msg);
  printf("msg : %s %ld\n",msg,strlen(msg));
  msg[msglen+1]='\r';
  msg[msglen+2]='\n';
  send(msocket,msg,msglen+2,0); // Send introduction
  printf("Enter messageges in formats : \n");
  printf("L~ --- Receive list of clients\n");
  printf("M [client_name] [message]~ --- send message to client\n");
  // Main client loop
  for(;;){
    printf("\n");
    getline(&buff,&bsize,stdin);
    //printf("input : %s \n",buff,strlen(buff));
    send(msocket,buff,strlen(buff),0);
  }

  return 0;
}
