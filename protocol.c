#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int serialize_game_message(const GameMessage *msg, char *buffer, size_t buffer_size)
{
    if (buffer_size < sizeof(GameMessage))
    {
        return -1; // Buffer too small
    }
    memcpy(buffer, msg, sizeof(GameMessage));
    return sizeof(GameMessage);
}

int desserialize_game_message(const char *buffer, size_t buffer_size, GameMessage *msg)
{
    if (buffer_size < sizeof(GameMessage))
    {
        return -1; // Buffer too small
    }
    memcpy(msg, buffer, sizeof(GameMessage));
    return sizeof(GameMessage);
}

void init_game_message(GameMessage *msg)
{
    msg->type = MSG_REQUEST;
    msg->client_action = 0;
    msg->server_action = 0;
    msg->result = 0;
    msg->client_win = 0;
    msg->server_win = 0;
    memset(msg->msg, 0, MSG_SIZE);
}

void print_game_message(const GameMessage *msg)
{
    printf("Message Type: %d\n", msg->type);
    printf("Client Action: %d\n", msg->client_action);
    printf("Server Action: %d\n", msg->server_action);
    printf("Result: %d\n", msg->result);
    printf("Client Win: %d\n", msg->client_win);
    printf("Server Win: %d\n", msg->server_win);
    printf("Message: %s\n", msg->msg);
}