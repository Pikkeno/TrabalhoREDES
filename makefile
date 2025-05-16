CC = gcc
CFLAGS = -Wall -Wextra -std=c99
BIN_DIR = bin

all: $(BIN_DIR)/client $(BIN_DIR)/server

$(BIN_DIR)/client: client.c common.c protocol.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) client.c common.c protocol.c -o $(BIN_DIR)/client

$(BIN_DIR)/server: server.c common.c protocol.c
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) server.c common.c protocol.c -o $(BIN_DIR)/server

clean:
	rm -rf $(BIN_DIR)
