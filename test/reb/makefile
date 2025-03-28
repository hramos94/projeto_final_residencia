.PHONY: clean reb aux

CC = gcc
#CFLAGS=-std=c99
CFLAGS += -Wall
CFLAGS += -Wextra
#CFLAGS += -Wpointer-arith
#CFLAGS += -Wcast-align
#CFLAGS += -Wwrite-strings
#CFLAGS += -Wswitch-default
#CFLAGS += -Wunreachable-code
#CFLAGS += -Winit-self
#CFLAGS += -Wmissing-field-initializers
#CFLAGS += -Wno-unknown-pragmas
#CFLAGS += -Wstrict-prototypes
#CFLAGS += -Wundef
#CFLAGS += -Wold-style-definition
CFLAGS += -coverage

INCLUDES = -I../unity/inc -I../../bsw/inc -I../../reb/inc


BSW_DIR = ../../bsw/src
REB_DIR = ../../reb/src
UNITY_DIR = test_runner
OBJ_DIR = obj
BIN_DIR = bin
TARGET_REB = $(BIN_DIR)/run_rest_reb


BSW_FILES = $(wildcard $(BSW_DIR)/*.c)
UNITY_FILES = $(wildcard test_runner/*.c)

REB_FILES = $(wildcard $(REB_DIR)/*.c)
REB_FILES := $(filter-out ../../reb/src/rte.c, $(REB_FILES))

OBJS_BSW = $(patsubst $(BSW_DIR)/%.c, $(OBJ_DIR)/%.o, $(BSW_FILES))
OBJS_UNITY = $(patsubst $(UNITY_DIR)/%.c, $(OBJ_DIR)/%.o, $(UNITY_FILES))
OBJS_REB = $(patsubst $(REB_DIR)/%.c, $(OBJ_DIR)/%.o, $(REB_FILES))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all:$(TARGET_REB)

run:
	./$(TARGET_REB)

$(OBJ_DIR)/%.o : $(BSW_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(REB_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(UNITY_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(TARGET_REB): $(OBJS_BSW) $(OBJ_DIR)/unity.o $(OBJS_UNITY) $(OBJS_REB)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(TARGET_REB) 
	./$(TARGET_REB)

$(OBJ_DIR)/unity.o:
	$(CC) $(CFLAGS) -c ../unity/src/unity.c -o $@ $(INCLUDES)

clean:
	rm -rf obj/ bin/


