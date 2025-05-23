#include "protocol.h"
#include "common.h"

#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

void logexit(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void usage(int argc, char **argv) {
    printf("Usage: %s <v4|v6> <server_port>\n", argv[0]);
    printf("Example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc < 3) {
        usage(argc, argv);
    }

    struct sockaddr_storage storage;
    if (server_sockaddr_init(argv[1], argv[2], &storage) != 0) {
        usage(argc, argv);
    }

    int s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) logexit("socket");

    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (bind(s, addr, sizeof(storage)) != 0) logexit("bind");
    if (listen(s, 10) != 0) logexit("listen");

    char addrstr[BUFFER_SIZE];
    addrtostr(addr, addrstr, BUFFER_SIZE);
    printf("Bound to %s, waiting for connections...\n", addrstr);

    srand(time(NULL)); 

    while (1) {
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);
        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1) logexit("accept");

        char caddrstr[BUFFER_SIZE];
        addrtostr(caddr, caddrstr, BUFFER_SIZE);
        printf("[log] Accepted connection from %s\n", caddrstr);

        int client_wins = 0;
        int server_wins = 0;

        while (1) {
            GameMessage msg;
            init_game_message(&msg);

            msg.type = MSG_REQUEST;
            send(csock, &msg, sizeof(msg), 0);

            recv(csock, &msg, sizeof(msg), 0);
            if (msg.type != MSG_RESPONSE || msg.client_action < 0 || msg.client_action > 4) {
                msg.type = MSG_ERROR;
                snprintf(msg.message, MSG_SIZE, "Por favor, selecione um valor de 0 a 4.");
                send(csock, &msg, sizeof(msg), 0);
                continue;
            }

            msg.server_action = rand() % 5;
           
            #tabela para comparar as escolhas feitas entre o client e servidor
            #solução diferenciada quero me gabar nao
            int tabela[5][5] = {
                { -1,  0,  1,  1,  0 },
                {  1, -1,  1,  1,  0 },
                {  0,  0, -1,  1,  1 },
                {  0,  0,  0, -1,  1 },
                {  1,  1,  0,  0, -1 }
            };

            
            int ca = msg.client_action;
            int sa = msg.server_action;
            msg.result = tabela[ca][sa];

            if (msg.result == 1) client_wins++;
            else if (msg.result == 0) server_wins++;

            msg.client_wins = client_wins;
            msg.server_wins = server_wins;

            msg.type = MSG_RESULT;
            snprintf(msg.message, MSG_SIZE,
                "Você escolheu %d, servidor escolheu %d. Resultado: %s.",
                ca, sa,
                msg.result == 1 ? "Vitória" :
                msg.result == 0 ? "Derrota" : "Empate"
            );

            send(csock, &msg, sizeof(msg), 0);

            if (msg.result == -1) continue;

            msg.type = MSG_PLAY_AGAIN_REQUEST;
            snprintf(msg.message, MSG_SIZE, "Deseja jogar novamente? (1 - Sim, 0 - Nao)");
            send(csock, &msg, sizeof(msg), 0);

            recv(csock, &msg, sizeof(msg), 0);
            if (msg.type != MSG_PLAY_AGAIN_RESPONSE || (msg.client_action != 0 && msg.client_action != 1)) {
                msg.type = MSG_ERROR;
                snprintf(msg.message, MSG_SIZE, "Digite 1 para sim ou 0 para nao.");
                send(csock, &msg, sizeof(msg), 0);
                continue;
            }

            if (msg.client_action == 0) {
                msg.type = MSG_END;
                snprintf(msg.message, MSG_SIZE,
                    "Jogo encerrado. Placar final - Cliente: %d, Servidor: %d",
                    client_wins, server_wins);
                send(csock, &msg, sizeof(msg), 0);
                break;
            }
        }

        close(csock); 
    }

    close(s); 
    return 0;
}
