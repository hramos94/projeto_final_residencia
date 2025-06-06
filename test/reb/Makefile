.PHONY: clean reb aux all

CC = gcc
CFLAGS += -g 
COVFLAGS = -coverage -fcondition-coverage

INCLUDES = -I../unity/inc -I../../bsw/inc -Imocks -I../../reb/inc

BSW_DIR = ../../bsw/src
REB_DIR = ../../reb/src
TEST_DIR = src
TEST_RUN_DIR = src/test_runners
OBJ_DIR = obj
BIN_DIR = bin
MOCK_DIR = mocks
TARGET_REB = $(BIN_DIR)/run_test_reb
UNITY_DIR = ../unity/src


BSW_FILES = $(BSW_DIR)/mcal.c $(BSW_DIR)/ecu.c  $(BSW_DIR)/dtc_logger.c

REB_FILES = $(REB_DIR)/ecu_reb.c $(REB_DIR)/app.c

MOCK_FILES = $(wildcard $(MOCK_DIR)/*c)

TEST_FILES = $(wildcard $(TEST_DIR)/*.c) 
TEST_RUN_FILES = $(wildcard $(TEST_RUN_DIR)/*.c) 
UNITY_FILES = $(wildcard $(UNITY_DIR)/*.c)


OBJS_BSW = $(patsubst $(BSW_DIR)/%.c, $(OBJ_DIR)/%.o, $(BSW_FILES))
OBJS_TEST = $(patsubst $(TEST_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_FILES))
OBJS_RUN_TEST = $(patsubst $(TEST_RUN_DIR)/%.c, $(OBJ_DIR)/%.o, $(TEST_RUN_FILES))
OBJS_UNITY = $(patsubst $(UNITY_DIR)/%.c, $(OBJ_DIR)/%.o, $(UNITY_FILES))
OBJS_MOCK = $(patsubst $(MOCK_DIR)/%.c, $(OBJ_DIR)/%.o, $(MOCK_FILES))
OBJS_REB = $(patsubst $(REB_DIR)/%.c, $(OBJ_DIR)/%.o, $(REB_FILES))

$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

all:$(TARGET_REB)

cov:
	$(MAKE) CFLAGS="$(CFLAGS) $(COVFLAGS)" all

run:
	@./$(TARGET_REB) -v

$(OBJ_DIR)/%.o : $(BSW_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES) 

$(OBJ_DIR)/%.o : $(REB_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(TEST_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(TEST_RUN_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(UNITY_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

$(OBJ_DIR)/%.o : $(MOCK_DIR)/%.c
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)


$(TARGET_REB): $(OBJS_BSW) $(OBJS_TEST) $(OBJS_UNITY) $(OBJS_RUN_TEST) $(OBJS_MOCK) $(OBJS_REB)
	@$(CC) $(CFLAGS) $(OBJ_DIR)/*.o -o $(TARGET_REB) 
	@make run

clean:
	@rm -rf obj/ bin/


