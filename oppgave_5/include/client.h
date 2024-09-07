#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAGIC_NUMBER 0x4A454449

typedef struct {
    int iMagicNumber;
    int iMessageSize;
    int iIpAddress;
    int iPhoneNumber;
} TASK5_CONNECT_USR;

typedef struct {
    int iMagicNumber;
    int iMessageSize;
    char szMessage[1024]; // 256
} TASK5_SENDMESSAGE;

/* Prototyper */
int main(int argc, char *argv[]);

#endif /* CLIENT_H */

