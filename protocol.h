#pragma once

#ifndef PROTOCOL_H
#define PROTOCOL_H

#define MSG_SIZE 1024

typedef enum {
    MSG_REQUEST,
    MSG_RESPONSE,
    MSG_RESULT,
    MSG_PLAY_AGAIN_REQUEST,
    MSG_PLAY_AGAIN_RESPONSE,
    MSG_ERROR,
    MSG_END
} MessageType;

typedef struct {
    int type;
    int client_action;
    int server_action;
    int result;
    int client_win;
    int server_win;
    char msg[MSG_SIZE];
} GameMessage;

#endif
