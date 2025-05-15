#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h
#include <unistd.h>

#include <arpa/inet.h>
#include <netinet/in.h>

#include "common.h"


#define BUFFER_SIZE 1024

void usage(int argc, char **argv)
{
    printf("Usage: %s <v4|v6> <server_port>\n", argv[0]);
    printf("example: %s v4 51511\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        logexit("socket");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        logexit("bind");
    }

    if (0 != listen(s, 10))
    {
        logexit("listen");
    }

    char addrstr[BUFFER_SIZE];
    addrtostr(addr, addrstr, BUFFER_SIZE);
    printf("bound to %s, waiting for connections...\n", addrstr);

    while (1)
    {
        struct sockaddr_storage cstorage struct sockaddr *caddr = (struct sockaddr *)(&cstorage);

        int csock = accept(s, caddr, sizeof(cstorage));
        if (csock == -1)
        {
            logexit("accept");
        }

        char caddrstr[BUFFER_SIZE];
        addrtostr(addr, caddrstr, BUFFER_SIZE);
        printf("[log] accepted connection from %s\n", caddrstr);

        char buf[BUFFER_SIZE];
        memset(buf, 0, BUFFER_SIZE);
        size_t count = recv(csock, buf, BUFFER_SIZE, 0);
        printf("[msg] %s, %d bytes: %s\n", caddrstr, (int)count, buf);

        sprintf(buf, "remote endpoint: %s", caddrstr);
        send(csock, buf, strlen(buf) + 1, 0);
        if (count != strlen(buf) + 1)
        {
            logexit("send");
        }
        close(csock);
    }
