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

INCLUDES = -Iunity/inc -I../bsw/inc 


DIR = ../bsw/src
UNITY_DIR = test_runner
OBJ_DIR = obj
BIN_DIR = bin
TARGET = $(BIN_DIR)/run_rest


SRC_FILES = $(wildcard $(DIR)/*.c)
SRC_UNITY = $(wildcard test_runner/*.c)
#SRCS_FILES := $(filter-out ../reb/src/rte.c, $(SRCS_FILES))

OBJS_FILES = $(patsubst $(DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

OBJS_UNITY = $(patsubst $(UNITY_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_UNITY))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all:$(TARGET)

$(OBJ_DIR)/%.o : $(DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(UNITY_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(TARGET): $(OBJS_FILES) $(OBJ_DIR)/unity.o $(OBJS_UNITY)
	$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(TARGET) 

$(OBJ_DIR)/unity.o:
	$(CC) $(CFLAGS) -c unity/src/unity.c -o $@ $(INCLUDES)

clean:
	rm -rf obj/ bin/

run:
	$(TARGET)

.PHONY: info clean all
