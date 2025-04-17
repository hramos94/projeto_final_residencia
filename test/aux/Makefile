.PHONY: clean reb aux

CC = gcc
CFLAGS = -g

COVFLAGS = -coverage -fcondition-coverage


INCLUDES = -I../unity/inc -I../../bsw/inc -I../../aux/inc -I./mocks


BSW_DIR = ../../bsw/src
AUX_DIR = ../../aux/src
TEST_DIR = src
TEST_RUN_DIR = src/test_runners
OBJ_DIR = obj
BIN_DIR = bin
TARGET_AUX = $(BIN_DIR)/run_rest_aux
UNITY_DIR = ../unity/src
MOCK_DIR = ./mocks


BSW_FILES = $(BSW_DIR)/mcal.c $(BSW_DIR)/ecu.c  $(BSW_DIR)/dtc_logger.c

MOCK_FILES = $(wildcard $(MOCK_DIR)/*c)

TEST_FILES = $(wildcard $(TEST_DIR)/*.c) 
TEST_RUN_FILES = $(wildcard $(TEST_RUN_DIR)/*.c) 
UNITY_FILES = $(wildcard $(UNITY_DIR)/*.c)

AUX_FILES = $(wildcard $(AUX_DIR)/*.c)
AUX_FILES := $(filter-out ../../aux/src/rte.c, $(AUX_FILES))

OBJS_BSW = $(patsubst $(BSW_DIR)/%.c, $(OBJ_DIR)/%.o, $(BSW_FILES))
OBJS_TEST = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_FILES))
OBJS_RUN_TEST = $(patsubst $(TEST_RUN_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_RUN_FILES))
OBJS_AUX = $(patsubst $(AUX_DIR)/%.c, $(OBJ_DIR)/%.o, $(AUX_FILES))
OBJS_UNITY = $(patsubst $(UNITY_DIR)/%.c, $(OBJ_DIR)/%.o, $(UNITY_FILES))

OBJS_MOCK = $(patsubst $(MOCK_DIR)/%.c, $(OBJ_DIR)/%.o, $(MOCK_FILES))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all:$(TARGET_AUX)

cov:
	$(MAKE) CFLAGS="$(CFLAGS) $(COVFLAGS)" all


run:
	@./$(TARGET_AUX) -v

$(OBJ_DIR)/%.o : $(BSW_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(AUX_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(TEST_RUN_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(UNITY_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(MOCK_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(TARGET_AUX): $(OBJS_BSW) $(OBJS_TEST) $(OBJS_AUX) $(OBJS_UNITY) $(OBJS_RUN_TEST) $(OBJS_MOCK)
	@$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(TARGET_AUX) 
	@make run

clean:
	@rm -rf obj/ bin/


