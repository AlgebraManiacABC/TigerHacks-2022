NAME=hack

CC=gcc
CFLAGS=-Wall -Werror -std=c17 -Ilibs
ifeq ($(OS),Windows_NT)
	CFLAGS += -lSDL2main -lSDL2
endif

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
	mkdir $(OBJ_DIR)

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(NAME)