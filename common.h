#pragma once

#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stddef.h>

#include <netinet/in.h>
#include <arpa/inet.h>

int addrparse(const char *addrstr, const char *portstr, struct sockaddr_storage *storage);
void addrtostr(const struct sockaddr *addr, char *str, size_t strsize);
void logexit(const char *msg);
int server_sockaddr_init(const char *proto, const char *portstr,
                         struct sockaddr_storage *storage);
#endif
