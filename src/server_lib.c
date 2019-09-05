#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <syslog.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include "server_lib.h"

struct client_struct{
  char name[255]; // Client name
  int con_sock; // Connection file descriptor
  int status; // Status of client
};


#define USAGE_STR "./server -q -p [portnumber]\n"
#define SERVER_NAME "KomSrv"
#define BSIZE 1024
#define LIMIT 10

void AddClient(struct client_struct** clist,char* name,int fd){
  if(strlen(name)){
    int i=0;
    while((*clist)[i].status != -1){
      if(!strcmp((*clist)[i].name,name) && (*clist)[i].status==0){
        (*clist)[i].con_sock = fd;
        (*clist)[i].status = 1;
        return;
      }
      i++;
    }
    i++;
    //strcat((*clist)[0].name,name );
    //syslog(0,"Clist : %s %d ",(*clist)[0].name,i);
    *clist = realloc(*clist,(i+1)*sizeof(struct client_struct));
    //(*clist)[i] = NULL;
    //syslog(0,"reallocind, i = %d",i);
    memset((*clist)[i-1].name,0,255);
    //syslog(0,"memsetind");
    strcat((*clist)[i-1].name, name);
    //syslog(0,"strcatind");
    (*clist)[i-1].con_sock = fd;
    (*clist)[i-1].status = 1;
    (*clist)[i].status=-1;
    syslog(0,"Client added to the list : %s,%d,%d",(*clist)[i-1].name,(*clist)[i-1].con_sock,(*clist)[i-1].status);
  }
  else{
    syslog(0,"Invalid client name");
  }
}

char* GetClientList(struct client_struct* clist){
  int i=0;
  char* ostr;
  ostr = (char*) calloc (BSIZE,1);
  while(clist[i].status != -1){
    if(clist[i].status){
      syslog(0,"adding name : %s",clist[i].name);
      strcat(ostr,clist[i].name);
      strcat(ostr,"\n");
    }
    i++;
  }
  syslog(0,"Created list : %s ",ostr);
  return ostr;
}

char* GetNameBySock(struct client_struct* clist, int fd){
  int i=0;
  while(clist[i].status != -1){
    if(clist[i].con_sock == fd){
      return  clist[i].name;
    }
    i++;

  }
  return NULL;
}

void RemoveClient(struct client_struct** clist, char* name){
  int i;
  while((*clist)[i].status != -1){
    if(!strcmp((*clist)[i].name,name)){
      (*clist)[i].status = 0;
      syslog(0,"Changed status to offline for client %s",name);
    }
    i++;
  }
}


void SendMsg(char* msg,struct client_struct* clist){
  char name[255];
  char tosend[BSIZE];
  memset(name,0,255);
  memset(tosend,0,BSIZE);
  int i=0,j=0;
  while(msg[i]!=' '){
    name[i]=msg[i];
    i++;
  }
  name[i]='\0';
  while(msg[i]!='~' && j<BSIZE){
    tosend[j]=msg[i];
    j++;
    i++;
  }
  msg[j]='\0';
  i=0;
  while(strcmp(name,clist[i].name)){
    i++;
  }
  send(clist[i].con_sock,tosend,strlen(msg),0);
}

int HandleOtherInstance(char flags){
  pid_t mpid = getpid();
  char* command;
  char out[20];
  command = (char*) calloc (1,100);
  sprintf(command,"pidof -o %d ./bin/server ",mpid);
  FILE* pidof = popen(command,"r");
  fgets(out,20,pidof);
  pid_t ipid= strtoul(out,NULL,10);
  if(ipid){
    printf("Obecnie działa instancja servera \n");
    kill(ipid,SIGKILL);
    if(!(flags&0x02))
      return 1;
  }
  free(command);
  return 0;
}
