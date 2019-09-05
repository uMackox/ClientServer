#include "client_lib.h"


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
