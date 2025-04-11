EXEC_NAME=rayalarm
CC=gcc
CFLAGS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11
BUILD_PATH= ./build
SRC_DIR = ./src

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%=$(BUILD_PATH)/%.o)

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