CC = gcc

TARGET = carnival

SRC_DIR = src
INCLUDE_DIR = include deps

BUILD_DIR = ./build

SRC_FILES = ./src/main.c

C_FLAGS = -Werror

INC_FILES = $(addprefix -I,$(INCLUDE_DIR))

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(INC_FILES) $(SRC_FILES) -o $@

make-run: $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) $(INC_FILES) $(SRC_FILES) -o $@
	$(BUILD_DIR)/$(TARGET)

profile: $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) -O2 -fprofile-instr-generate $(INC_FILES) $(SRC_FILES) -o $(BUILD_DIR)/$(TARGET)

profile-run:
	$(BUILD_DIR)/$(TARGET) -fprofile-instr-use

run:
	$(BUILD_DIR)/$(TARGET)

gpu-build: $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(C_FLAGS) -DGPU -L/opt/local/lib/libomp/ -lomp -Xpreprocessor -O1 $(INC_FILES) -I/opt/local/include/libomp/ $(SRC_FILES) -o $(BUILD_DIR)/$(TARGET)

.PHONY: $(BUILD_DIR)/$(TARGET) clean
clean:
	rm -r $(BUILD_DIR)
