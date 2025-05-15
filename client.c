#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

#define MSG_SIZE 1024
#define SERVER_PORT 51511

void usage(int argc, char **argv)
{
    fprintf(stderr, "Usage: %s <server_ip> <server_port>\n");
    exit(EXIT_FAILURE);
}

void logexit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}

#define BUFSZ 1024

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        usage();
    }

    int s;
    s = socket(storage->ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        logexit("socket");
    }

    struct sockaddr_storage storage;
    if (0 != addrparse(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != connect(s, addr, sizeof(storage)))
    {
        logexit("connect");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);

    printf("Connected to %s\n", addrstr);

    char buf[BUFSZ];
    memset(buf, 0, BUFSZ);
    printf("mensagem> ");
    fgets(buf, BUFSZ, stdin);
    int count = send(s, buf, strlen(buf) + 1, 0);
    if (count != strlen(buf) + 1)
    {
        logexit("send");
    }

    memset(buf, 0, BUFSZ);
    unsigned total = 0;
    while (1)
    {
        count = recv(s, buf + total, BUFSZ - total, 0);
        if (count == 0)
            break;
    }

    close(s);

    exit(EXIT_SUCCESS);
    return 0;
}
