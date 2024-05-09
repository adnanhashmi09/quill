CC = gcc
LDFLAGS = -lncurses
DEPFLAGS = -MP -MD
CFLAGS = -Wall -Wextra -std=c11 $(DEPFLAGS)

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)

SOURCES = $(wildcard $(SRC_DIR)/**/*.c $(SRC_DIR)/*.c)
TARGET = $(BUILD_DIR)/quill

OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

SHARED_SOURCES = $(wildcard $(SRC_DIR)/**/*.c)
SHARED_OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SHARED_SOURCES))

TEST_DIR = tests
TEST_BUILD_DIR = $(OBJ_DIR)/$(TEST_DIR)
TEST_SOURCES = $(wildcard $(TEST_DIR)/**/*.c $(TEST_DIR)/*.c)
TEST_TARGET = $(TEST_BUILD_DIR)/test
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c,$(TEST_BUILD_DIR)/%.o,$(TEST_SOURCES))

LIB_DIR = lib
LIB_INCLUDE_DIR = $(LIB_DIR)/include
LIB_SOURCES = $(wildcard $(LIB_DIR)/**/*.c)
LIB_OBJECTS = $(patsubst $(LIB_DIR)/%.c,$(OBJ_DIR)/lib/%.o,$(LIB_SOURCES))

INCLUDE_FILES = $(wildcard $(INCLUDE_DIR)/*.h, $(LIB_INCLUDE_DIR)/*.h)

all: $(TARGET)
run: $(TARGET)
	$(TARGET)

$(TARGET): $(OBJECTS) $(INCLUDE_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -I$(INCLUDE_DIR) -o $@


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c 
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $@

run-tests: test
	$(TEST_TARGET)
test: $(TEST_TARGET) $(INCLUDE_FILES)

$(TEST_TARGET): $(TEST_OBJECTS) $(SHARED_OBJECTS) $(LIB_OBJECTS)
	@mkdir -p $(TEST_BUILD_DIR)
	$(CC) $(CFLAGS) $(TEST_OBJECTS) $(SHARED_OBJECTS) $(LIB_OBJECTS) $(LDFLAGS) -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR) -o $@

$(OBJ_DIR)/lib/%.o: $(LIB_DIR)/%.c
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR) -c $< -o $@

$(TEST_BUILD_DIR)/%.o: $(TEST_DIR)/%.c 
	@mkdir -p $$(dirname $@)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -I$(LIB_INCLUDE_DIR) -c $< -o $@

$(TEST_BUILD_DIR):
	mkdir -p $@

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)

DEPS=$(wildcard $(OBJ_DIR)/**/*.d $(OBJ_DIR)/*.d, $(OBJ_DIR)/**/**/*.d)
ifneq ($(DEPS),)
include $(DEPS)
endif
