NAME=hack

CC=gcc
CFLAGS=-Wall -Werror -std=c17 -Ilibs

LIB_DIR=libs
SRC_DIR=srcs
OBJ_DIR=build

LIBS=$(wildcard $(LIB_DIR)/*.h)
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJ_A=$(SRCS:%.c=%.o)
OBJS=$(OBJ_A:$(SRC_DIR)%=$(OBJ_DIR)%)

$(NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBS) | $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(NAME)