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

void* ReceiveMsgs(void* arg);
