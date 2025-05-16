#include "protocol.h"
#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSZ 1024

void usage(void) {
    fprintf(stderr, "Uso: ./client <server_ip> <server_port>\n");
    exit(EXIT_FAILURE);
}

void logexit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        usage();
    }

    const char *server_ip = argv[1];
    const char *server_port = argv[2];

    struct sockaddr_storage storage;
    if (addrparse(server_ip, server_port, &storage) != 0) {
        usage();
    }

    int sock = socket(storage.ss_family, SOCK_STREAM, 0);
    if (sock == -1) {
        logexit("socket");
    }

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (connect(sock, addr, sizeof(storage)) != 0) {
        logexit("connect");
    }

    char addrstr[BUFSZ];
    addrtostr(addr, addrstr, BUFSZ);
    printf("Conectado ao servidor %s\n", addrstr);

    while (1) {
        GameMessage msg;
        memset(&msg, 0, sizeof(msg));

        // Espera mensagem do servidor
        ssize_t count = recv(sock, &msg, sizeof(msg), 0);
        if (count <= 0) {
            printf("Conexão encerrada pelo servidor.\n");
            break;
        }

        switch (msg.type) {
            case MSG_REQUEST:
                printf("\nEscolha sua jogada:\n");
                printf("0 - Nuclear\n");
                printf("1 - Internet\n");
                printf("2 - Cibernético\n");
                printf("3 - Drones\n");
                printf("4 - Biológico\n");
                printf("Digite sua escolha: ");
                int escolha;
                scanf("%d", &escolha);
                while (getchar() != '\n'); // limpa buffer

                msg.type = MSG_RESPONSE;
                msg.client_action = escolha;
                send(sock, &msg, sizeof(msg), 0);
                break;

            case MSG_RESULT:
                printf("\n--- Resultado da Rodada ---\n");
                printf("%s\n", msg.message);
                printf("Placar: Cliente %d x %d Servidor\n", msg.client_wins, msg.server_wins);
                break;

            case MSG_PLAY_AGAIN_REQUEST:
                printf("\n%s\n", msg.message);
                int resposta;
                printf("Digite 1 para sim ou 0 para não: ");
                scanf("%d", &resposta);
                while (getchar() != '\n');

                msg.type = MSG_PLAY_AGAIN_RESPONSE;
                msg.client_action = resposta;
                send(sock, &msg, sizeof(msg), 0);
                break;

            case MSG_ERROR:
                printf("\n[ERRO] %s\n", msg.message);
                break;

            case MSG_END:
                printf("\n%s\n", msg.message);
                close(sock);
                return 0;

            default:
                printf("\n[AVISO] Mensagem desconhecida do servidor.\n");
                break;
        }
    }

    close(sock);
    return 0;
}
