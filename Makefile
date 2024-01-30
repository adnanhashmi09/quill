CC = gcc
CFLAGS = -Wall -Wextra -std=c11
LDFLAGS = -lncurses
SRC_DIR = .
BUILD_DIR = build

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))

quill: $(OBJ_FILES)
	$(CC) -o $(BUILD_DIR)/quill $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

.PHONY: clean run
clean:
	rm -rf $(BUILD_DIR)

run:
	./build/quill
