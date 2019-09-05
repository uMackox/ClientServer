CLIENT_TARGET=client
SERVER_TARGET=server
CLIENT_SRC=client_main.c
SERVER_SRC=server_main.c
CLIENT_LIB=server
SERVER_LIB=

SRC_DIR=./src
BIN_DIR=./bin
TMP_DIR=./tmp
BUILD_FLAGS = -Wall
CFLAGS = -Wall -c
CLIENT_OBJ= $(TMP_DIR)/client.o $(TMP_DIR)/client_main.o
SERVER_OBJ= $(TMP_DIR)/server.o $(TMP_DIR)/server_main.o

CC=gcc

$(CLIENT_TARGET): dirs $(CLIENT_OBJ)
	$(CC) $(BUILD_FLAGS) -o $(BIN_DIR)/$(CLIENT_TARGET) $(CLIENT_OBJ) -lpthread
$(SERVER_TARGET): dirs $(SERVER_OBJ)
	$(CC) $(BUILD_FLAGS) -o $(BIN_DIR)/$(SERVER_TARGET) $(SERVER_OBJ)
$(CLIENT_OBJ): $(SRC_DIR)/$(CLIENT_SRC)
	$(CC) $(CFLAGS) -o $(SRC_DIR)/$(CLIENT_SRC) $
$(SERVER_OBJ): $(SRC_DIR)/$(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER_OBJ) $(SRC_DIR)/$(SERVER_SRC)

dirs:
	mkdir -p $(BIN_DIR) $(TMP_DIR)
clean:
	rm -rf $(BIN_DIR) $(TMP_DIR)
