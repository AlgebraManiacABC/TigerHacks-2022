NAME=hack

CC=gcc
CFLAGS=-Wall -Werror -std=c17 -Ilibs -lgSDL
ifeq ($(OS),Windows_NT)
	CFLAGS += -lmingw32 -llibSDL2_image
else
	CFLAGS += -lSDL2_image
endif
	CFLAGS += -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer

LIB_DIR=libs
SRC_DIR=srcs
OBJ_DIR=build
OUT_DIR=bin

LIBS=$(wildcard $(LIB_DIR)/*.h)
SRCS=$(wildcard $(SRC_DIR)/*.c)
OBJ_A=$(SRCS:%.c=%.o)
OBJS=$(OBJ_A:$(SRC_DIR)%=$(OBJ_DIR)%)

$(OUT_DIR)/$(NAME): $(OBJS) | $(OUT_DIR)
	$(CC) -o $@ $^ $(CFLAGS)

$(OUT_DIR):
	mkdir bin

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(LIBS) | $(OBJ_DIR)
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: clean

clean:
	$(RM) -r $(OBJ_DIR)
	$(RM) $(OUT_DIR)/$(NAME)