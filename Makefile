CC = gcc
CFLAGS = -Wall -g -Wextra -I./src -I./src/utils -I./src/functionalities \
         -I./src/functionalities/ErmesGraphical -I./src/functionalities/record \
         -I./src/functionalities/reproduce -I./src/functionalities/identify \
         -I./src/functionalities/compress -I./src/functionalities/export \
         -I./src/functionalities/management -I./src/functionalities/database
LDFLAGS = -lsqlite3 -lsndfile -lncurses
SRC_DIR = src
OBJ_DIR = build
BIN = ermes_app.exe

SRCS = $(shell find $(SRC_DIR) -name "*.c")
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) $(BIN)

rebuild: clean all

.PHONY: all clean rebuild
