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

INCLUDES = -I../unity/inc -I../../bsw/inc -I../../aux/inc


BSW_DIR = ../../bsw/src
AUX_DIR = ../../aux/src
UNITY_DIR = test_runner
OBJ_DIR = obj
BIN_DIR = bin
TARGET_AUX = $(BIN_DIR)/run_rest_aux


BSW_FILES = $(wildcard $(BSW_DIR)/*.c)
UNITY_FILES = $(wildcard test_runner/*.c)

REB_FILES = $(wildcard $(AUX_DIR)/*.c)
REB_FILES := $(filter-out ../../aux/src/rte.c, $(REB_FILES))

OBJS_BSW = $(patsubst $(BSW_DIR)/%.c, $(OBJ_DIR)/%.o, $(BSW_FILES))
OBJS_UNITY = $(patsubst $(UNITY_DIR)/%.c, $(OBJ_DIR)/%.o, $(UNITY_FILES))
OBJS_AUX = $(patsubst $(AUX_DIR)/%.c, $(OBJ_DIR)/%.o, $(REB_FILES))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all:$(TARGET_AUX)

run:
	./$(TARGET_AUX)

$(OBJ_DIR)/%.o : $(BSW_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(AUX_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(UNITY_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(TARGET_AUX): $(OBJS_BSW) $(OBJ_DIR)/unity.o $(OBJS_UNITY) $(OBJS_AUX)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(TARGET_AUX) 
	./$(TARGET_AUX)

$(OBJ_DIR)/unity.o:
	$(CC) $(CFLAGS) -c ../unity/src/unity.c -o $@ $(INCLUDES)

clean:
	rm -rf obj/ bin/


