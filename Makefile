CC = gcc

TARGET = carnival

SRC_DIR = src
INCLUDE_DIR = include

BUILD_DIR = ./build

SRC_FILES = $(shell find $(SRC_DIR) -name *.c)

C_FLAGS = -Wall -Werror

INC_FILES = $(addprefix -I,$(INCLUDE_DIR))

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(INC_FILES) $(SRC_FILES) -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)
