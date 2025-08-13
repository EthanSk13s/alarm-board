EXEC_NAME=alarm-board
CC=gcc
CFLAGS=-O2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -lpigpio -lcurl -I./libs/
BUILD_PATH= ./build
SRC_DIR = ./src
LIB_DIR = ./libs

LIBS := $(shell find $(LIB_DIR) -name '*.c')
SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(SRCS:%=$(BUILD_PATH)/%.o) $(LIBS:%=$(BUILD_PATH)/%.o)

$(BUILD_PATH)/$(EXEC_NAME): $(OBJS)
	$(CC) $(OBJS) -o $@ $(CFLAGS)

$(BUILD_PATH)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CFLAGS) -c $< -o $@

run: $(BUILD_PATH)/$(EXEC_NAME)
	./$(BUILD_PATH)/$(EXEC_NAME)

clean: $(BUILD_PATH)/$(EXEC_NAME)
	rm -r $(BUILD_PATH)/$(SRC_DIR)
	rm -f $(BUILD_PATH)/$(EXEC_NAME)