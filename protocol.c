#include "protocol.h"
#include <stdio.h>
#include <string.h>

void init_game_message(GameMessage *msg) {
    if (!msg) return;
    memset(msg, 0, sizeof(GameMessage));
    msg->type = MSG_REQUEST;
}

void print_game_message(const GameMessage *msg) {
    if (!msg) return;
    printf("Message Type: %d\n", msg->type);
    printf("Client Action: %d\n", msg->client_action);
    printf("Server Action: %d\n", msg->server_action);
    printf("Result: %d\n", msg->result);
    printf("Client Wins: %d\n", msg->client_wins);
    printf("Server Wins: %d\n", msg->server_wins);
    printf("Message: %s\n", msg->message);
}
