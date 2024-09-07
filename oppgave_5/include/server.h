#ifndef SERVER_H
#define SERVER_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <netinet/in.h>

#define MAGIC_NUMBER 0x4A454449
#define MAX_MESSAGE_SIZE 1048576  /* Max størrelse 1 MB */

/* Strukt for håndtering av tilkoblin */
typedef struct {
    int iMagicNumber;
    int iMessageSize;
    int iIpAddress;
    int iPhoneNumber;
} TASK5_CONNECT_USR;

/* Strukt for meldingen som blir sendt */
typedef struct {
    int iMagicNumber;
    int iMessageSize;
    char szMessage[];
} TASK5_SENDMESSAGE;

/* Prototyper */
void *handleConnection(void *sockFdPtr);
int setupServer(int argc, char *argv[]);

#endif /* SERVER_H */

